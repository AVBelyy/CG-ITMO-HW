import Data.Char
import Data.List
import System.IO
import Control.Applicative
import Control.Monad
import qualified Data.ByteString.Lazy.Char8 as BS

data Point a = P { x :: a, y :: a }
data BST a = BST { runBST :: BS.ByteString -> (a, BS.ByteString) }

instance Functor BST where
    fmap f b = BST $ \s -> let (a, s') = runBST b s in (f a, s')

instance Applicative BST where
    pure = return
    (<*>) = ap

instance Monad BST where
    return a = BST $ \s -> (a, s)
    f >>= g = BST $ \s -> let (a, s') = runBST f s in runBST (g a) s'

readInt :: BST Int
readInt = BST $ \s -> let Just (a, s') = BS.readInt s in (a, BS.dropWhile isSpace s')

readPoint :: BST (Point Int)
readPoint = do
    x <- readInt
    y <- readInt
    return (P x y)

instance Show a => Show (Point a) where
    show p = show (x p) ++ " " ++ show (y p)

instance Eq a => Eq (Point a) where
    P x1 y1 == P x2 y2 = x1 == x2 && y1 == y2

instance Num a => Num (Point a) where
    P x1 y1 - P x2 y2 = P (x1 - x2) (y1 - y2)

preparePoints :: Integral a => [Point a] -> [Point a]
preparePoints pts = p0 : sortBy (compareByTurn p0) pts where
    leftmost (P x1 y1) (P x2 y2) = if x1 == x2 then compare y1 y2 else compare x1 x2
    p0 = minimumBy leftmost pts

turn :: (Ord a, Num a) => Point a -> Point a -> Point a -> a
turn a b c = (x b - x a) * (y c - y a) - (y b - y a) * (x c - x a)

compareByTurn :: (Ord a, Num a) => Point a -> Point a -> Point a -> Ordering
compareByTurn p0@(P x0 y0) p1@(P x1 y1) p2@(P x2 y2)
    | turn p0 p1 p2 > 0  = LT
    | turn p0 p1 p2 == 0 = compare ((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0)) ((x2-x0)*(x2-x0) + (y2-y0)*(y2-y0))
    | otherwise          = GT

convexHull :: (Ord a, Num a) => [Point a] -> [Point a]
convexHull (p0:p1:rest) = process [p1, p0] (rest ++ [p0]) where
    process (_:st) [] = st
    process (b:a:st) (c:pts)
        | turn a b c > 0  = process (c:b:a:st) pts
        | turn a b c == 0 = process (c:a:st) pts
        | otherwise       = process (a:st) (c:pts)

getPoints :: BST [Point Int]
getPoints = do
    n <- readInt
    replicateM n readPoint

getPointsFromFile :: Handle -> IO [Point Int]
getPointsFromFile handle = do
    bytes <- BS.hGetContents handle
    return $ fst $ runBST getPoints bytes

main = do
    points <- getPointsFromFile stdin
    let hull = convexHull (preparePoints points)
    putStrLn $ show (length hull)
    putStrLn $ intercalate "\n" (map show hull)
