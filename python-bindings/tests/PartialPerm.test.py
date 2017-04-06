import unittest
import sys
import os

path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
if not path in sys.path:
    sys.path.insert(1, path)
del path

from semigroups import PartialPerm,Transformation


class TestPartialPerm(unittest.TestCase):
    def test_init(self):
        PartialPerm([1,0,2],[2,0,1],3)
        PartialPerm([1,0],[0,1],5)
        PartialPerm([0,3,4,5,8,20,23373],[1,2,34,23423,233,432,26],26260)


    def test_init_fail(self):
        with self.assertRaises(ValueError):
            PartialPerm([1,3],[0,1],3)
            PartialPerm([1,2],[3,2],3)
            PartialPerm([-2,2],[0,1],3)
            PartialPerm([1,2],[-1,2],3)
            PartialPerm([1,2],[2,2],3)
            PartialPerm([1,1],[0,2],3)
            PartialPerm([],[],-1)

        with self.assertRaises(TypeError):
            PartialPerm([1,2],[0,'i'],3)
            PartialPerm([1,[0]],[1,2],3)
            PartialPerm([0,1],[2,3],[4])
            PartialPerm([0,1],[2,3],4.3)

        with self.assertRaises(IndexError):
            PartialPerm([1,2],[0,1,2],3)


    def test_mul(self):
        self.assertEqual( PartialPerm([0, 1], [0, 1], 2)*PartialPerm([0, 1], [0, 1], 2), PartialPerm([0, 1], [0, 1], 2))
        self.assertEqual( PartialPerm([1, 2, 4, 6, 7, 23], [0, 5, 2, 4, 6, 7], 26)*PartialPerm([2, 4, 3, 5, 0, 19], [7, 8, 2, 3, 23, 0], 26), PartialPerm([1, 2, 4, 6], [23, 3, 7, 8], 26))
        self.assertEqual( PartialPerm([0, 3, 7, 2], [5, 7, 1, 3], 8)*PartialPerm([4, 7, 3, 6], [5, 0, 3, 2], 8), PartialPerm([2, 3], [3, 0], 8))

        with self.assertRaises(AssertionError):
            PartialPerm([1,2],[0,1],3)*PartialPerm([1,2],[0,1],4)
            PartialPerm([0,1],[0,1],2)*Transformation([0,1])

    def test_pow(self):
        self.assertEqual( PartialPerm([0, 1], [0, 1], 2)**20, PartialPerm([0, 1], [0, 1], 2))
        self.assertEqual( PartialPerm([1, 2, 4, 6, 7, 23], [0, 5, 2, 4, 6, 7], 26)**5, PartialPerm([4, 6, 7, 23], [5, 2, 4, 6], 26))
        self.assertEqual( PartialPerm([0, 3, 7, 2], [5, 7, 1, 3], 8)**10, PartialPerm([], [], 8))

        with self.assertRaises(AssertionError):
            PartialPerm([1,2],[0,1],3)**-1

    def test_iter(self):
        self.assertEqual( list(PartialPerm([0, 1], [0, 1], 2)), [0, 1])
        self.assertEqual( list(PartialPerm([0,1,3],[1,2,4],5)), [1, 2, 65535, 4, 65535])
        self.assertEqual( list(PartialPerm([0, 3, 7, 2], [5, 7, 1, 3], 8)), [5, 65535, 3, 7, 65535, 65535, 65535, 1])

    def test_rank(self):
        self.assertEqual(PartialPerm([1,4,2],[2,3,4],6).rank(),3)
        self.assertEqual(PartialPerm([1, 2, 4, 6, 7, 23], [0, 5, 2, 4, 6, 7], 26).rank(),6)
        
        with self.assertRaises(TypeError):
            PartialPerm([1,2],[0,1],3).rank(2)







if __name__ == '__main__':
    unittest.main()