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
import math

#Add _handle.right_cayley_graph
#Add Gabow's algm
#Change _handle to _cpp_semigroup or _cpp_trans,.. etc

#cdef class MyCppElement(cpp.Element):
#    pass

cdef class __dummyClass:
    def __init__(self):
        pass

cdef class Element:# Add identity
    """
    An abstract base class for handles to libsemigroups elements.

    Any subclass shall implement an ''__init__'' method which
    initializes _handle.
    """
    cdef cpp.Element* _handle

    def __cinit__(self):
        self._handle = NULL

    def __dealloc__(self):
        """
        Deallocate the memory of the element.

        Args:
            None (although if using the usual 'del' format, the element must
            be given after 'del').

        Returns:
            None

        Raises:
            TypeError:  If any args given.

        Example:
            >>> from semigroups import Transformation, PartialPerm
            >>> p = PartialPerm([0, 1, 3], [2, 1, 0], 4)
            >>> del p
            >>> p in globals()
            >>> 'p' in globals()
            False

        """
        if self._handle != NULL:
            self._handle[0].really_delete()
            del self._handle

    def __mul__(Element self, Element other):
        """
        Function for computing the product of two elements

        Args:
            other (Element):    The element to be multiplied by. If using the
                                usual a * b format, then the args will be two
                                objects of Element class.

        Returns:
            Element:    The product of the two elements given. This will be the
                        same type as the two elements.

        Raises:
            TypeError:  If elements are not the same type
            ValueError: If elements have different degrees

        Example:
            >>> from semigroups import Transformation
            >>> x = Transformation([2, 1, 1])
            >>> y = Transformation([2, 1, 0])
            >>> x * y
            [0, 1, 1]
            >>> y * x
            [1, 1, 2]
        """
        if not isinstance(self, type(other)):
            raise TypeError('Elements must be same type')
        cdef cpp.Element* product = self._handle.identity()
        if self._handle.degree() != other._handle.degree():
            raise ValueError('Element degrees must be equal')
        product.redefine(self._handle, other._handle)
        return self.new_from_handle(product)
	
    def __pow__(self, power, modulo):#It works, but don't understand why it needs 'modulo' argument aal20
        """
        Function for multiplying an element by itself a number of times.

        Args:
            power (int):    The number of times to multiply the element by
                            itself.

            modulo:         This argument is not used.

        Returns:
            Element:    The product of the elements when multiplied by istelf
                        power times.

        Raises:
            TypeError:  If power is not an 'int'.
            ValueError: If power is not positive.

        Example:
            >>> from semigroups import PartialPerm
            >>> PartialPerm([1, 2, 4], [0, 2, 3], 5) ** 3
            PartialPerm([2], [2], 5)
        """
        if not(isinstance(power, int)):
            raise TypeError('Can only power by int')
        if power < 0:
            raise ValueError('Power must be positive')

        #Converts power to binary, then constructs element to the power of 2^n for needed n.
        binaryString = bin(power - 1)[2:]
        powerOf2List = [self]
        for x in binaryString:
            powerOf2List.append(powerOf2List[-1].__mul__(powerOf2List[-1]))
        output = self 

	#generates answer using element to the power of powers of 2 (binary tells you which ones to multiply)
        for i in range(len(binaryString)):
            if binaryString[i] == "1":
                 output = output.__mul__(powerOf2List[i])
        return output
    
    def __richcmp__(Element self, Element other, int op):
        """
        Function for comparing elements using the total order defined on the
        elements of a semigroup.

        Args:
            other (Element):    The element to be compared with.

            op (int):           The type of comparison used. Note that in the
                                usual format a < b, a == b, etc., this will be
                                calculated automatically.

        Returns:
            bool:    Whether or not the given comparison is true.

        Raises:
            TypeError:  If elements are not of the same type.
            ValueError: If elements have different degree

        Examples:
            >>> from semigroups import Transformation, PartialPerm
            >>> Transformation([1, 2, 2, 3]) < Transformation([1, 3, 2, 2])
            True
            >>> PartialPerm([1, 2], [1, 2], 3) == PartialPerm([1, 2], [2, 1], 3)
            False
        """

        if not isinstance(self, type(other)):
            raise TypeError('Elements must be same type')

        if op == 0:
            return self._handle[0] < other._handle[0]
        elif op == 1:
            return self._handle[0] < other._handle[0] or self._handle[0] == other._handle[0]
        elif op == 2:
            return self._handle[0] == other._handle[0]
        elif op == 3:
            return not self._handle[0] == other._handle[0]
        elif op == 4:
            return not (self._handle[0] < other._handle[0] or self._handle[0] == other._handle[0])
        elif op == 5:
            return not self._handle[0] < other._handle[0]

    # TODO: Make this a class method
    cdef new_from_handle(self, cpp.Element* handle):
        """
        Construct a new element from a specified handle and with the
        same class as ``self``.
        """
        cdef Element result = self.__class__(__dummyClass)
        result._handle = handle[0].really_copy()
        return result

    def degree(self):
        """
        Function for finding the degree of an element.

        This method returns an integer which represents the size of an element,
        and is used to determine whether or not two elements are compatible for
        multiplication.

        Args:
            None

        Returns:
            int: The degree of the element

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import PartialPerm
            >>> PartialPerm([1, 2, 5], [2, 3, 5], 6).degree()
            6
        """
        return self._handle.degree()

    def identity(self):
        """
        Function for finding the mutliplicative identity

        This function finds the multiplicative identity of the same element
        type and degree as the current element.

        Args:
            None

        Returns:
            Element: The identity element of the Element subclass

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import PartialPerm
            >>> PartialPerm([0, 2], [1, 2], 3).identity()
            PartialPerm([0, 1, 2], [0, 1, 2], 3)
        """
        cdef cpp.Element* identity = self._handle.identity()
        return self.new_from_handle(identity)

cdef class Transformation(Element):
    """
    A class for handles to libsemigroups transformations.

    A transformation f is a function defined on the set {0, 1, ..., n - 1}
    for some integer n called the degree of f. A transformation is stored as a
    list of the images of {0, 1, ..., n   -1},
    i.e. [(0)f, (1)f, ..., (n - 1)f].

    Args:
        List (list): Image list of the Transformation when applied to
        [0, 1, ..., n]

    Raises:
        TypeError:  If the arg is not a list of ints, or if there is more than
                    one arg.
        ValueError: If the elements of the list are negative, or the max of the
                    list + 1 is greater than the length of the list. 

    Example:
        >>> from semigroups import Transformation
        >>> Transformation([2, 1, 1])
        Transformation([2, 1, 1])        
    """
    def __init__(self, List):
        
        if List is not __dummyClass:
            for i in List:
                if not isinstance(i, int):
                    raise TypeError('Image list must only contain ints')
                if i < 0:
                    raise ValueError('Image list cannot contain negative values')
            if not isinstance(List, list):
                raise TypeError('Input must be a list')
            if max(List) + 1 > len(List):
                raise ValueError('Not a valid Transformation')
            self._handle = new cpp.Transformation[uint16_t](List)

    def __iter__(self):
        """
        Function for iterating through the image list of 'self'.

        Args:
            None

        Returns:
            generator:  A generator of the image list.

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import Transformation
            >>> list(Transformation([1, 2, 0]))
            [1, 2, 0]
        """
        cdef cpp.Element* e = self._handle
        e2 = <cpp.Transformation[uint16_t] *>e
        for x in e2[0]:
            yield x

    def __repr__(self):
        """
        Function for printing a string representation of 'self'.
        
        Args:
            None

        Returns:
            str: 'Transformation' then the image list in parenthesis.

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import Transformation
            >>> Transformation([1, 2, 0])
            Transformation([1, 2, 0])
        """
        return "Transformation(" + str(list(self)) + ")"

cdef class PartialPerm(Element):
    """
    A class for handles to libsemigroups partial perm.

    A partial permutation f is an injective partial transformation, which is
    stored as a list of the images of {0, 1, ..., n -1}, i.e. 
    [(0)f, (1)f, ..., (n - 1)f] where the value -1 is used to indicate i(f) is
    undefined.

    Args: Can pass either of the following
        args (list):    Image list of the partial permutation when applied to
                        [0, 1, ..., n], -1 being used to indicate the image is
                        undefined.

        args (list):    List containing the domain, as a list of ints, then the
                        range, as a list of ints, then the degree.

    Raises:
        TypeError:  During the second arg format, if the domain or range are
                    not both lists, the degree is not an int, or the elements
                    of the domain and range are not ints.
        ValueError: If the domain and range are of different lengths, if the
                    degree is negative, if the domain or range contains an
                    element greater than or equal to the degree, or if the
                    domain or range have repeats.

    Example:
        >>> from semigroups import PartialPerm
        >>> PartialPerm([1, 2, 5], [2, 3, 5], 6)
        PartialPerm([1, 2, 5], [2, 3, 5], 6)
    """
    
    cdef list _domain,_range
    
    def __init__(self, *args):

        if len(args) == 1:
            if args[0] == __dummyClass:
                return
            self._handle = new cpp.PartialPerm[uint16_t](list(args)[0])
        else:
            if not isinstance(args[0], list):
                raise TypeError('Domain must be a list')
            if not isinstance(args[1], list):
                raise TypeError('Range must be a list')
            if not isinstance(args[2], int):
                raise TypeError('Degree must be an int')

            self._domain, self._range, _degree = args[0], args[1], args[2]

            if _degree < 0:
                raise ValueError('Degree must be non-negative')
            if len(self._domain) != len(self._range):
                raise ValueError('Domain and range must be same size')
            if len(self._domain) != 0:
                if max(max(self._domain),max(self._range)) >= _degree:
                    raise ValueError('The max of the domain and range must \
                    be strictly less than the degree')

            n = len(self._domain)
            imglist = [65535] * _degree

            for i in range(n):
                if not (isinstance(self._domain[i], int) and \
                isinstance(self._range[i], int)):
                    raise TypeError('Elements of domain and range must be ints')
                if self._domain[i] < 0 or self._range[i] < 0:
                    raise ValueError('Elements of domain and range must be \
                    non-negative')
                
                #Ensures range and domain have no repeats
                if self._range[i] in imglist:
                    raise ValueError('Range cannot have repeats')
                if self._domain.count(i) > 1:
                    raise ValueError('Domain cannot have repeats')

            for i in range(n):
                imglist[self._domain[i]] = self._range[i]

            self._handle = new cpp.PartialPerm[uint16_t](imglist)

    def _generator(self):
        cdef cpp.Element* e = self._handle
        e2 = <cpp.PartialPerm[uint16_t] *>e
        for x in e2[0]:
            yield x

    def _init_dom_ran(self):
        if self._domain == None or self._range == None:
            L = list(self._generator())
            self._domain, self._range = [], []
            for i in range(self.degree()):
                if L[i] != 65535 and L[i] != -1:
                    self._domain.append(i)
                    self._range.append(L[i])

    def __repr__(self):
        """
        Function for printing a string representation of a partial permutation.
        
        Args:
            None

        Returns:
            str: 'PartialPerm' then the domain, range, degree in parenthesis.

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import PartialPerm
	        >>> PartialPerm([1, 4, 2], [2, 3, 4], 6)
	        PartialPerm([1, 4, 2], [2, 3, 4], 6)
        """

        self._init_dom_ran()
        return ("PartialPerm(%s, %s, %s)"%(self._domain, self._range,\
        self.degree())).replace('65535', '-1')

    def rank(self):
        """
        Function for finding the rank of the partial permutation.

        Args:
            None

        Returns:
            int: The rank of the partial permutation

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import PartialPerm
            >>> PartialPerm([1, 2, 5], [2, 3, 5], 6).rank()
            3
        """
        cdef cpp.Element* e = self._handle
        e2 = <cpp.PartialPerm[uint16_t] *>e
        return e2.crank()

    def domain(self):
        """
        Function for finding the domain of the partial permutation, that
        maps to defined elements.

        Args:
            None

        Returns:
            list: The domain of the partial permutation

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import PartialPerm
            >>> PartialPerm([1, 2, 5], [2, 3, 5], 6).domain()
            [1, 2, 5]
        """
        self._init_dom_ran()
        return self._domain

    def range(self):
        """
        Function for finding the range of the partial permutation.

        Args:
            None

        Returns:
            list: The range of the partial permutation

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import PartialPerm
            >>> PartialPerm([1, 2, 5], [2, 3, 5], 6).range()
            [2, 3, 5]
        """
        self._init_dom_ran()
        return self._range

cdef class Bipartition(Element):
    """
    A class for handles to libsemigroups bipartition. 

    A bipartition is a partition of the set {-n, ..., -1} union {1, ..., n}
    for some integer n. This can be stored as a list of blocks, the subsets
    of the bipartition

    Args: Can pass either of the following
        args (lists):   The blocks of the bipartition as lists.

    Raises:
        TypeError:  If any of the blocks are not lists
        ValueError: If the union of the blocks is not the set {-n, ..., -1}
                    union {1, ..., n}

    Example:
        >>> from semigroups import Bipartition
        >>> Bipartition([1, -1], [2, 3, -2], [-3])
        Bipartition([1, -1], [2, 3, -2], [-3])
    """

    cdef list _blocks

    def __init__(self, *args):
        if args[0] is not __dummyClass:

            n = 1

            for sublist in args:
                if not isinstance(sublist, list):
                    raise TypeError('Arguments must be lists')
                n = max(max(sublist), n)

            #Note that this assert ensures all entries are non-zero ints
            if set().union(*args) != \
            set(range(1, n + 1)).union(set(range(-1, -n - 1, -1))):
                raise ValueError('Not a valid Biparition')

            argsCopy = []
            self._blocks = []

            for sublist in args:
                self._blocks.append(sublist[:])
                argsCopy.append(sublist[:])

            for sublist in argsCopy:
                for i in range(len(sublist)):
                    entry = sublist[i]
                    sublist[i] = n + abs(entry) - 1 if entry < 0 else entry - 1
                sublist.sort()

            argsCopy.sort()
            output = [0] * n * 2
            for i, sublist in enumerate(argsCopy):
                for j in sublist:
                    output[j] = i

            self._handle = new cpp.Bipartition(output)

    def _generator(self):
        cdef cpp.Element* e = self._handle
        e2 = <cpp.Bipartition *>e
        for x in e2[0]:
            yield x

    def _init_blocks(self):
        if self._blocks is None:
            self._blocks = []
            gen = set(self._generator())
            i = 0
            n = 2 * self.degree()
            while i in gen:
                block = []
                for ind, j in enumerate(self._generator()):
                    if j == i:
                        if ind < n/2:
                            block.append(ind + 1)
                        else:
                            block.append(int(n/2 - ind - 1))
                self._blocks.append(block)
                i += 1
                            
    def blocks(self):
        """
        Function for finding the blocks of a bipartition

        Args:
            None

        Returns:
            list: The blocks of the bipartition

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import Bipartition
            >>> Bipartition([1, 2], [-2, -1, 3], [-3]).blocks()
            [[1, 2], [-2, -1, 3], [-3]]
        """
        self._init_blocks()
        return self._blocks

    def numberOfBlocks(self):
        """
        Function for finding the number of blocks of a bipartition.

        Args:
            None

        Returns:
            int: The number blocks of the bipartition

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import Bipartition
            >>> Bipartition([1, 2], [-2, -1, 3], [-3]).numberOfBlocks()
            3
        """
        cdef cpp.Element* e = self._handle
        e2 = <cpp.Bipartition *>e
        return e2.const_nr_blocks()

    def block(self, element):
        """
        Function for finding the index of the block that a given element is in.

        The blocks are ordered by lowest element absolute value,
        where all negative elements are greater than all positive elements.

        Args:
            element (int): The element in question

        Returns:
            int: The index of the block that the element is in

        Raises:
            ValueError: If the element is not in the bipartition

        Example:
            >>> from semigroups import Bipartition
            >>> Bipartition([1, 2], [-2, -1, 3], [-3]).block(-2)
            1
        """

        n = self.degree()
        if not element in set(range(1, n + 1)).union(set(range(-1, -n - 1, -1))):
            raise ValueError('Element not in Bipartition')
        
        if element < 1:
            element = n - element - 1
        else:
            element -= 1
        
        cdef cpp.Element* e = self._handle
        e2 = <cpp.Bipartition *>e
        return e2.block(element)

    def isTransverseBlock(self, index):
        """
        Function for finding whether a given block is transverse.

        A block is transverse if it contains both positive and negative
        elements. The blocks are ordered by lowest element absolute value,
        where all negative elements are greater than all positive elements.

        Args:
            index (int): The index of the block in question

        Returns:
            list: The blocks of the bipartition

        Raises:
            TypeError:  If index is not an int.
            IndexError: If index does not relate to the index of a block in the
                        partition

        Example:
            >>> from semigroups import Bipartition
            >>> Bipartition([1, 2], [-2, -1, 3], [-3]).isTransverseBlock(1)
            True
        """
        if not isinstance(index, int):
            raise TypeError("Index must be 'int' type")

        if index < 0 or abs(index) > self.numberOfBlocks() - 1:
            raise IndexError('Index out of range')
        cdef cpp.Element* e = self._handle
        e2 = <cpp.Bipartition *>e
        return e2.is_transverse_block(index)

    def __repr__(self):
        """
        Function for printing a string representation of the bipartition.
        
        Args:
            None

        Returns:
            str: 'Bipartition' then the blocks in parenthesis.

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import Bipartition
            >>> Bipartition([1, -1], [2], [-2])
            Bipartition([1, -1], [2], [-2])
        """
        self._init_blocks()
        return 'Bipartition(%s)'%self._blocks.__repr__()[1:-1]

#Write semiring class in python.

cdef class BooleanMat(Element):#Add 0s, 1s
    """
    A class for handles to libsemigroups BooleanMat. 

    A boolean matrix is a matrix with entries either True or False.

    Args: Can pass either of the following
        args (lists):   The rows of the matrix as lists.

    Raises:
        TypeError:  If any of the rows are not lists

        ValueError: If the number of lists given does not equal the length of 
                    every list

    Example:
        >>> from semigroups import BooleanMat
        >>> BooleanMat([True, True], [False, True])
        BooleanMat([1, 1], [0, 1])
    """

    cdef list _rows, _int_rows

    def __init__(self, *args):
        
        if args[0] is not __dummyClass:
            n = len(args)
            S = set([0, 1])

            for row in args:
                if not isinstance(row, list):
                    raise TypeError
                if len(row) != n:
                    raise ValueError

            t = type(args[0][0])

            for row in args:
                for entry in row:
                    if not entry in S:
                        raise TypeError
                    if not isinstance(entry, t):
                        raise TypeError

            self._rows = []
            self._int_rows = []
            
            booldict = {0: False, 1: True}
            #Have to use dict, since python bool function doesn't seem to work

            if t == int:
                for row in args:
                    self._int_rows.append(row[:])
                    tempRow = []
                    for entry in row:
                        tempRow.append(booldict[entry])
                    self._rows.append(tempRow)

            else:
                for row in args:
                    self._rows.append(row[:])
                    tempRow = []
                    for entry in row:
                        tempRow.append(int(entry))
                    self._int_rows.append(tempRow)

            self._handle = new cpp.BooleanMat(self._rows)

    def _generator(self):
        cdef cpp.Element* e = self._handle
        e2 = <cpp.BooleanMat *>e
        for x in e2[0]:
            yield x

    def _init_rows(self):
        if self._rows is not None:
            return
        n = self.degree()
        self._rows = []
        row = []
        for i,entry in enumerate(self._generator()):
            if i % n == 0 and i !=0:
                self._rows.append(row)
                row = []
            row.append(entry)
        self._rows.append(row)

    def _init_int_rows(self):
        if self._int_rows is not None:
            return
        n = self.degree()
        self._int_rows = []
        row = []
        for i,entry in enumerate(self._generator()):
            if i % n == 0 and i !=0:
                self._int_rows.append(row)
                row = []
            row.append(int(entry))
        self._int_rows.append(row)

    def rows(self):
        """
        Function for finding the rows of a boolean matrix.

        Args:
            None

        Returns:
            list: The rows of the boolean matrix.

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import BooleanMat
            >>> BooleanMat([True, False], [True, True]).rows()
            [[True, False], [True, True]]
        """

        self._init_rows()
        return self._rows

    def int_rows(self):
        """
        Function for finding the rows (as 0s or 1s) of a boolean matrix.

        Args:
            None

        Returns:
            list: The rows of the boolean matrix.

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import BooleanMat
            >>> BooleanMat([True, False], [True, True]).rows()
            [[1, 0], [1, 1]]
        """

        self._init_int_rows()
        return self._int_rows

    def __repr__(self):
        """
        Function for printing a string representation of the boolean matrix.
        
        Args:
            None

        Returns:
            str: 'BooleanMat' then the rows in parenthesis.

        Raises:
            TypeError:  If any argument is given.

        Example:
            >>> from semigroups import BooleanMat
            >>> BooleanMat([1, 1], [0, 0])
            BooleanMat([1, 1], [0, 0])
        """

        self._init_int_rows()
        return "BooleanMat(%s)"%self._int_rows.__repr__()[1:-1]

cdef class PBR(Element):
    """
    A class for handles to libsemigroups PBR. 

    A partitioned binary relation is a generalisation of a Bipartition, where
    elements are adjacent to some other elements, but a adjacent to b need not
    imply b adjacent to a.

    Args: Can pass either of the following
        args (lists):   The adjacencies of the negative elements as a list of
                        lists followed by the positive elements as a list of
                        lists

    Raises:
        TypeError:  If more less than two argments are given, if the given
                    arguments are not lists.

        ValueError: If there are a different number of positive and negative
                    elements included in the adjacencies, if any element is
                    adjacent to another element twice, if an element is
                    adjacent to an element not in the set.

    Example:
        >>> from semigroups import PBR
        >>> PBR([[1], [1, 2, -1]], [[1], [2, -1, 1]])
        PBR([[1], [1, 2, -1]], [[1], [2, -1, 1]])
    """

    cdef list _negativeAdjacencies, _positiveAdjacencies

    def __init__(self, *args):

        if args[0] is not __dummyClass:
            
            if len(args) != 2:
                raise TypeError('Expected two arguments, the negative and' + \
                                'positive adjacencies, received %s'%len(args))

            if not (isinstance(args[0], list) and isinstance(args[1], list)):
                raise TypeError('Adjacencies must be lists')

            n = len(args[0])

            if n != len(args[1]):
                raise ValueError('Must have same number of positive and' + \
                                 'negative adjacencies')

            for sublist in args[0] + args[1]:
                if max(abs(min(sublist)), max(sublist)) > n or 0 in sublist:
                    raise ValueError('Adjacency given outside possible set')
                if len(sublist) != len(set(sublist)):
                    raise ValueError('Ajacencies cannot be repeated')

            output = []
            self._positiveAdjacencies = []
            self._negativeAdjacencies = []

            for i,sublist in enumerate(args[1]):
                self._positiveAdjacencies.append(sublist[:])
                tempSublist = sublist[:]
                for j,entry in enumerate(sublist):
                    if entry < 0:
                        tempSublist[j] = n - entry - 1
                    else:
                        tempSublist[j] = entry - 1
                output.append(tempSublist)

            for i,sublist in enumerate(args[0]):
                self._negativeAdjacencies.append(sublist[:])
                tempSublist = sublist[:]
                for j,entry in enumerate(sublist):
                    if entry < 0:
                        tempSublist[j] = n - entry - 1
                    else:
                        tempSublist[j] = entry - 1
                output.append(tempSublist)

            self._handle = new cpp.PBR(output)

    def _generator(self):
        cdef cpp.Element* e = self._handle
        e2 = <cpp.PBR *>e
        for x in e2[0]:
            yield x

    def _init_adjacencies(self):
        if self._negativeAdjacencies is None or \
           self._positiveAdjacencies is None:
            n = self.degree()
            self._negativeAdjacencies, self._positiveAdjacencies = [], []
            for i,sublist in enumerate(self._generator()):
                newSublist = []
                for entry in sublist:
                    if entry < n:
                        newSublist.append(entry + 1)
                    else:
                        newSublist.append(n - entry - 1)
                if i < n:
                    self._positiveAdjacencies.append(newSublist)
                else:
                    self._negativeAdjacencies.append(newSublist)

    def __repr__(self):
        self._init_adjacencies()
        return "PBR(%s, %s)"%(self._negativeAdjacencies.__repr__(), self._positiveAdjacencies.__repr__())
    

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
    cdef cpp.Semigroup* _handle      
    # holds a pointer to the C++ instance which we're wrapping
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

    def nridempotents(self):
        return self._handle.nridempotents()
    
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

cdef class Semiring:

    cdef float _minus_infinity, _plus_infinity

    def __init__(self):
        self._minus_infinity = -math.inf
        self._plus_infinity = math.inf

cdef class Integers(Semiring):
    
    def __init___(self):
        pass

    def plus(self, x, y):
        if not (isinstance(x, int) and isinstance(y, int)):
            raise TypeError 
        return x + y

    def prod(self, x, y):
        if not (isinstance(x, int) and isinstance(y, int)):
            raise TypeError
        return x * y

    def zero(self):
        return 0

    def one(self):
        return 1

cdef class MaxPlusSemiring(Semiring):
    
    def __init___(self):
        pass

    def plus(self, x, y):
        if not ((isinstance(x, int) or x == -math.inf)  and (isinstance(y, int) or y == -math.inf)):
            raise TypeError
        return max(x, y)

    def prod(self, x, y):
        if not ((isinstance(x, int) or x == -math.inf)  and (isinstance(y, int) or y == -math.inf)):
            raise TypeError
        return x + y

    def zero(self):
        return self._minus_infinity

    def one(self):
        return 0

cdef class MinPlusSemiring(Semiring):
    
    def __init___(self):
        pass

    def plus(self, x, y):
        if not ((isinstance(x, int) or x == math.inf)  and (isinstance(y, int) or y == math.inf)):
            raise TypeError
        return min(x, y)

    def prod(self, x, y):
        if not ((isinstance(x, int) or x == math.inf)  and (isinstance(y, int) or y == math.inf)):
            raise TypeError
        return x + y

    def zero(self):
        return self._plus_infinity

    def one(self):
        return 0

def FullTransformationMonoid(n):
    assert isinstance(n, int) and n >= 1
    if n == 1: 
        return Semigroup(Transformation([0]))
    elif n == 2:
        return Semigroup(Transformation([1, 0]), Transformation([0, 0]))
    
    return Semigroup([Transformation([1, 0] + list(range(2, n))), 
                      Transformation([0, 0] + list(range(2, n))), 
                      Transformation([n - 1] + list(range(n - 1)))])

