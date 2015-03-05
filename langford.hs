{-# LANGUAGE TupleSections       #-}
{-# LANGUAGE ScopedTypeVariables #-}

import Control.Applicative ((<$>))
import qualified Data.Map as Map
import Data.List (unfoldr)

import System.Environment
    
main :: IO ()
main = do
  [(n :: Int)] <- map read <$> getArgs
  mapM_ (print . map snd . Map.toList) $ gl n

gl :: Int -> [M]
gl n = generateLangford n (Just ([], 1, [2..n])) Map.empty
        
type M = Map.Map Int Int

cleanM :: Int -> M
cleanM n = Map.empty

addItem :: Int -> Int -> Int -> M -> Maybe M
addItem n k v db =
    let k' = k + 1 + v
    in if k' > 2*n
       then Nothing
       else
           case (Map.lookup k db, Map.lookup k' db) of
             (Nothing, Nothing) -> Just . Map.insert k' v . Map.insert k v $ db
             _ -> Nothing

nextFree :: Int -> Int -> M -> Int
nextFree n k db =
    case Map.lookup k db of
      Nothing -> k
      _ -> if 2*n == k
           then error "should not get here"
           else nextFree n (k + 1) db

generateLangford :: Int -> Maybe ZipList -> M -> [M]
generateLangford n Nothing db = [db]
generateLangford n (Just l) db =
    let k = nextFree n 1 db
        combinations = map extract . allZipLists $ l
        tf db (v, Nothing) = maybe [] return . addItem n k v $ db
        tf db (v, Just l)  = case addItem n k v db of
                               Nothing -> []
                               Just db' -> generateLangford n (Just l) db'
    in concatMap (tf db) combinations

-- lists that are like zippers in some sense:

type ZipList = ([Int], Int, [Int])

next :: ZipList -> ZipList
next (bw, x, y:fw) = (x:bw, y, fw)

prev :: ZipList -> ZipList
prev (x:bw, y, fw) = (bw, x, y:fw)

allZipLists :: ZipList -> [ZipList]
allZipLists l@([], x, fw) = unfoldr generator (Just l)
    where generator (Just l@(_, _, [])) = Just (l, Nothing)
          generator (Just l           ) = Just (l, Just (next l))
          generator Nothing             = Nothing
allZipLists l = allZipLists (prev l)                                          

extract :: ZipList -> (Int, Maybe ZipList)
extract ([], x, []  ) = (x, Nothing)
extract (y:ys, x, zs) = (x, Just (ys, y, zs))
extract (ys, x, z:zs) = (x, Just (ys, z, zs))
