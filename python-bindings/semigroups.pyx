#cython: infer_types=True, embedsignature=True
"""
Python bindings for the libsemigroups C++ library.

`libsemigroups <https://github.com/james-d-mitchell/libsemigroups/>`_
is a C++ mathematical library for computing with finite `semigroups
<https://en.wikipedia.org/wiki/Semigroup>`_. This Cython module
provides bindings to call it from Python.

We construct the semigroup generated by `0` and `-1`::

    >>> from semigroups import Semigroup
    >>> S = Semigroup([0,-1])
    >>> S.size()
    3

We construct the semigroup generated by `0` and complex number `i`::

    >>> S = Semigroup([0, 1j])
    >>> S.size()
    5
"""
from libc.stdint cimport uint16_t
from libc.stdint cimport uint32_t
from libcpp.vector cimport vector
cimport semigroups_cpp as cpp
from libcpp cimport bool


#Add _handle.right_cayley_graph
#Add Gabow's algm
#Change _handle to _cpp_semigroup or _cpp_trans,.. etc

#cdef class MyCppElement(cpp.Element):
#    pass

cdef class __dummyClass:
    def __init__(self):
        pass
   

cdef class Element:
    """
    An abstract base class for handles to libsemigroups elements.

    Any subclass shall implement an ``__init__`` method which
    initializes _handle.

    .. WARNING::

        For now, the ``__init__`` method should also accept to be
        called with ``None`` as argument, in which case it *should
        not* initialize the handle.

        This is used by ``new_from_handle``.

    .. TODO::

        Find a better protocol to create an instance from a class and
        a handle.
    """
    cdef cpp.Element* _handle

    def __cinit__(self):
        self._handle = NULL

    def __dealloc__(self):
        """
        Deallocate the handle of ``self``.

        TESTS::

            >>> from semigroups import Semigroup, PythonElement, Transformation
            >>> x = PythonElement(-1)
            >>> x = 3

            >>> x = Transformation([1,2,0])
            >>> del x
        """
        if self._handle != NULL:
            self._handle[0].really_delete()
            del self._handle

    def __mul__(Element self, Element other):
        """
        Return the product of ``self`` and ``other``.

        EXAMPLES::

            >>> from semigroups import Semigroup, PythonElement, Transformation
            >>> x = Transformation([2,1,1])
            >>> y = Transformation([2,1,0])
            >>> x * y
            [0, 1, 1]
            >>> y * x
            [1, 1, 2]
        """
        assert isinstance(self,type(other))
        cdef cpp.Element* product = self._handle.identity()
        assert self._handle.degree() == other._handle.degree()
        product.redefine(self._handle, other._handle)
        return self.new_from_handle(product)
	
    def __pow__(self,power,modulo):#It works, but don't understand why it needs 'modulo' argument aal20
        assert isinstance(power,int)
        assert power>0

        #Converts power to binary, then constructs element to the power of 2^n for needed n.
        binaryString=bin(power-1)[2:]
        powerOf2List=[self]
        for x in binaryString:
            powerOf2List.append(powerOf2List[-1].__mul__(powerOf2List[-1]))
        output=self

	#generates answer using element to the power of powers of 2 (binary tells you which ones to multiply)
        for i in range(len(binaryString)):
            if binaryString[i]=="1":
                 output=output.__mul__(powerOf2List[i])
        return output

        '''
        >>> from semigroups import Semigroup, PythonElement, Transformation
        >>> y=Transformation([2,1,0])
        >>> y**2
        Transformation([0, 1, 2])
        >>> y*y
        Transformation([0, 1, 2])'''
    
    def __richcmp__(Element self, Element other, int op):
        """Ref: http://docs.cython.org/src/userguide/special_methods.html#rich-comparisons"""
        if op==2:
            # op==2 is __eq__() in pure python
            if self._handle[0] == other._handle[0]:
                return True
            return False
        elif op==0:
            if self._handle[0] < other._handle[0]:
                return True
            return False
        # TODO more comparisons!
        else:
            err_msg = "op {0} isn't implemented yet".format(op)
            raise NotImplementedError(err_msg)
    
    # TODO: Make this a class method
    cdef new_from_handle(self, cpp.Element* handle):
        """
        Construct a new element from a specified handle and with the
        same class as ``self``.
        """
        cdef Element result = self.__class__(__dummyClass)
        result._handle = handle[0].really_copy()
        return result

cdef class Transformation(Element):#Add dealloc

#Python googlestyle docstrings for examples
    """
    A class for handles to libsemigroups transformations.

    EXAMPLES::

        >>> from semigroups import Semigroup, PythonElement, Transformation
        >>> Transformation([2,1,1])
        [2, 1, 1]
    """
    def __init__(self, List):
        
        if List is not __dummyClass:
            if not isinstance(List,list):
                raise TypeError('Input must be a list')
            if max(List)+1>len(List):
                raise ValueError('Not a valid Transformation')
            self._handle = new cpp.Transformation[uint16_t](List)



    def __iter__(self):
        """
        Return an iterator over ``self``

        EXAMPLES::

            >>> from semigroups import Transformation
            >>> list(Transformation([1,2,0]))
            [1, 2, 0]
        """
        cdef cpp.Element* e = self._handle
        e2 = <cpp.Transformation[uint16_t] *>e
        for x in e2[0]:
            yield x

    def __repr__(self):
        """
        Return a string representation of `self`.

        EXAMPLES::

            >>> from semigroups import Transformation
            >>> Transformation([1,2,0])
            [1, 2, 0]
        """
        return "Transformation(" + str(list(self)) + ")"



cdef class PartialPerm(Element):
    """
    A class for handles to libsemigroups partial perm.
    """
    
    cdef list _domain,_range
    cdef int _degree
    
    def __init__(self, *args):

        if len(args) == 1:
            if args[0] == __dummyClass:
                return
            self._handle = new cpp.PartialPerm[uint16_t](list(args)[0])
        else:

            if not (isinstance(args[0],list) and isinstance(args[1],list) and isinstance(args[2],int)):
                raise TypeError#Add error msgs

            self._domain, self._range, self._degree = args[0], args[1], args[2]
            

            if self._degree<0:#Seperate checking from definition
                raise ValueError
            if len(self._domain) != len(self._range):
                raise ValueError
            if len(self._domain)!=0:
                if not(max(self._domain) < self._degree and max(self._range) < self._degree):
                    raise ValueError
            imglist = [65535] * self._degree#Make imglist self.
            for i in range(len(self._domain)):
                if not (isinstance(self._domain[i],int) and isinstance(self._range[i],int)):
                    raise TypeError
                if self._domain[i]<0 or self._range[i]<0:
                    raise ValueError
                
                #Ensures range and domain have no repeats
                if self._range[i] in imglist:
                    raise ValueError
                if self._domain.count(i)>1:
                    raise ValueError

                imglist[self._domain[i]]=self._range[i]

            self._handle = new cpp.PartialPerm[uint16_t](imglist)

    def __iter__(self):#Remove
        cdef cpp.Element* e = self._handle
        e2 = <cpp.PartialPerm[uint16_t] *>e
        for x in e2[0]:
            yield x

    def init_dom_ran_deg(self):
        if self._domain==None or self._range==None or self._degree==None:
            L=list(self)
            self._degree, self._domain, self._range = len(L), [] ,[]
            for i in range(self._degree):
                if L[i] != 65535 and L[i] != -1:
                    self._domain.append(i)
                    self._range.append(L[i])

    def __repr__(self):
        """
        Return a string representation of `self`.

        EXAMPLES::

            >>> from semigroups import *
	    >>> PartialPerm([1,4,2],[2,3,4],6)
	    PartialPerm([1, 4, 2], [2, 3, 4], 6)


        """


        self.init_dom_ran_deg()
        return ("PartialPerm(%s, %s, %s)"%(self._domain,self._range,self._degree)).replace('65535','-1')

    def rank(self):
        cdef cpp.Element* e = self._handle
        e2 = <cpp.PartialPerm[uint16_t] *>e
        return e2.crank()

    def domain(self):
        self.init_dom_ran_deg()
        return self._domain

    def range(self):
        self.init_dom_ran_deg()
        return self._range

    def degree(self):
        self.init_dom_ran_deg()
        return self._degree

cdef class Bipartition(Element):
    """
    A class for handles to libsemigroups bipartition.
    """

    cdef list blocks

    def __init__(self,*args):#Copy args. Seperate.
        if args[0] is not __dummyClass:
            self.blocks=[]
            n=1
            for sublist in args:
                if not isinstance(sublist,list):
                    raise TypeError
                n=max(max(sublist),n)
                self.blocks.append(sublist[:])

            #Note that this assert ensures all entries are non-zero ints
            if set().union(*args)!=set(range(1,n+1)).union(set(range(-1,-n-1,-1))):
                raise ValueError

            

            dictOfSublistsWithMins={}
            for sublist in args:
                for i in range(len(sublist)):
                    entry=sublist[i]
                    if entry<0:
                        sublist[i]=n+abs(entry)
                dictOfSublistsWithMins[min(sublist)]=sublist
            output=[0]*(n*2)
            i=0

            while len(dictOfSublistsWithMins)>0:
                sublistKey=min(dictOfSublistsWithMins.keys())
                for item in dictOfSublistsWithMins[sublistKey]:
                    output[item-1]=i
                i+=1
                del dictOfSublistsWithMins[sublistKey]
            self._handle = new cpp.Bipartition(output)


    def __iter__(self):#Remove
        cdef cpp.Element* e = self._handle
        e2 = <cpp.Bipartition *>e
        for x in e2[0]:
            yield x

    def init_blocks(self):
        if self.blocks is None:
            self.blocks=[]
            List=list(self)
            i=0
            n=len(List)
            while i in List:
                block=[]
                for ind,j in enumerate(List):
                    if j==i:
                        if ind<n/2:
                            block.append(ind+1)
                        else:
                            block.append(n/2-ind-1)
                self.blocks.append(block)
                i+=1
                            
    def blocksList(self):#Rename blocks
        self.init_blocks()
        return self.blocks


    def IntRep(self):#Remove
        """
        Return a string representation of `self`.

        EXAMPLES::

            >>> from semigroups import *
            >>> p.IntRep()
            'Bipartition([1, 2, 2, 1, 2, 3])'


        """


        return "Bipartition(" + str(list(self)) + ")"

    def __repr__(self):
        self.init_blocks()
        return 'Bipartition(%s)'%self.blocks.__repr__()[1:-1]





cdef class PythonElement(Element):
    """
    A class for handles to libsemigroups elements that themselves wrap
    back a Python element

    EXAMPLE::

        >>> from semigroups import Semigroup, PythonElement
        >>> x = PythonElement(-1); x
        -1

        >>> Semigroup([PythonElement(-1)]).size()
        2
        >>> Semigroup([PythonElement(1)]).size()
        1
        >>> Semigroup([PythonElement(0)]).size()
        1
        >>> Semigroup([PythonElement(0), PythonElement(-1)]).size()
        3

        x = [PythonElement(-1)]
        x = 2

        sage: W = SymmetricGroup(4)
        sage: pi = W.simple_projections()
        sage: F = FiniteSetMaps(W)
        sage: S = Semigroup([PythonElement(F(p)) for p in pi])
        sage: S.size()
        23

    TESTS::

        Testing reference counting::

            >>> s = "UN NOUVEL OBJET"
            >>> sys.getrefcount(s)
            2
            >>> x = PythonElement(s)
            >>> sys.getrefcount(s)
            3
            >>> del x
            >>> sys.getrefcount(s)
            2
    """
    def __init__(self, value):
        if value is not None:
            self._handle = new cpp.PythonElement(value)

    def get_value(self):
        """

        """
        return (<cpp.PythonElement *>self._handle).get_value()

    def __repr__(self):
        return repr(self.get_value())

cdef class Semigroup:# Add asserts
    """
    A class for handles to libsemigroups semigroups

    EXAMPLES:

    We construct the symmetric group::

        >>> from semigroups import Semigroup, Transformation
        >>> S = Semigroup([Transformation([1,2,0]),Transformation([2,1,0])])
        >>> S.size()
        6
    """
    cdef cpp.Semigroup* _handle      # holds a pointer to the C++ instance which we're wrapping
    cdef Element _an_element

    def __cinit__(self):
        self._handle = NULL

    def __init__(self, generators):
        """
        TESTS::

            >>> Semigroup([1, Transformation([1,0])])
            ...
            TypeError: all generators should have the same type
        """
        generators = [g if isinstance(g, Element) else PythonElement(g)
                      for g in generators]
        if not len({type(g) for g in generators}) <= 1:
            raise TypeError("all generators should have the same type")
        cdef vector[cpp.Element *] gens
        for g in generators:
            gens.push_back((<Element>g)._handle)
        self._handle = new cpp.Semigroup(gens)
        self._an_element = generators[0]

    def __dealloc__(self):
        del self._handle

    def current_max_word_length(self):
        return self._handle.current_max_word_length()

    def nr_idempotents(self):
        return self._handle.nr_idempotents()
    
    def is_done(self):
        return self._handle.is_done()
    
    def is_begun(self):
        return self._handle.is_begun()
    
    def current_position(self, Element x):
        pos = self._handle.current_position(x._handle)
        if pos == -1:
            return None # TODO Ok?
        return pos
    
    def __contains__(self, Element x):
        return self._handle.test_membership(x._handle)

    def set_report(self, val):
        if val == True:
            self._handle.set_report(1)
        else:
            self._handle.set_report(0)

    def factorisation(self, Element x):
        '''
        >>> import from semigroups *
        >>> S = FullTransformationMonoid(5)
        >>> S.factorisation(Transformation([0] * 5))
        [1, 0, 2, 1, 0, 2, 1, 0, 2, 1]
        >>> S[1] * S[0] * S[2] * S[1] * S[0] * S[2] * S[1] * S[0] * S[2] * S[1]
        '''
        pos = self._handle.position(x._handle)
        if pos == -1:
            return None # TODO Ok?
        cdef vector[size_t]* c_word = self._handle.factorisation(pos)
        assert c_word != NULL
        py_word = [letter for letter in c_word[0]]
        del c_word
        return py_word
    
    def enumerate(self, limit):
        self._handle.enumerate(limit)

    def size(self):
        """
        Return the size of this semigroup

        EXAMPLES::

            >>> from semigroups import Semigroup, Transformation
            >>> S = Semigroup([Transformation([1,1,4,5,4,5]),Transformation([2,3,2,3,5,5])])
            >>> S.size()
            5
        """
        # Plausibly wrap in sig_off / sig_on
        return self._handle.size()

    cdef new_from_handle(self, cpp.Element* handle):
        return self._an_element.new_from_handle(handle)

    def __getitem__(self, size_t pos):
        """
        Return the ``pos``-th element of ``self``.

        EXAMPLES::

            >>> from semigroups import Semigroup
            >>> S = Semigroup([1j])
            >>> S[0]
            1j
            >>> S[1]
            (-1+0j)
            >>> S[2]
            (-0-1j)
            >>> S[3]
            (1-0j)
        """
        cdef cpp.Element* element
        element = self._handle.at(pos)
        if element == NULL:
            return None
        else:
            return self.new_from_handle(element)

    def __iter__(self):
        """
        An iterator over the elements of self.

        EXAMPLES::

            >>> from semigroups import Semigroup
            >>> S = Semigroup([1j])
            >>> for x in S:
            ...     print(x)
            1j
            (-1+0j)
            (-0-1j)
            (1-0j)
        """
        cdef size_t pos = 0
        cdef cpp.Element* element
        while True:
            element = self._handle.at(pos)
            if element == NULL:
                break
            else:
                yield self.new_from_handle(element)
            pos += 1

def FullTransformationMonoid(n):
    assert isinstance(n, int) and n >= 1
    if n == 1: 
        return Semigroup(Transformation([0]))
    elif n == 2:
        return Semigroup(Transformation([1, 0]), Transformation([0, 0]))
    
    return Semigroup([Transformation([1, 0] + list(range(2, n))), 
                      Transformation([0, 0] + list(range(2, n))), 
                      Transformation([n - 1] + list(range(n - 1)))])

