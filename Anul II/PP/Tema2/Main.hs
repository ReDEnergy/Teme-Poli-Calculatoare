module Main where

import System.Environment (getArgs)

import Env
import Types

main = do
  args <- getArgs
  maze <- readMaze $ head args
  envMain maze

readMaze :: FilePath -> IO Maze
readMaze fileName = readFile fileName >>= parseMaze

parseMaze :: String -> IO Maze
parseMaze fileContent = do
  let vals = map (map read . words) $ lines fileContent
  let (width, height) = (length vals, length $ head vals)
  let zipRow x = zipWith (\y v -> ((x, y), v)) [0..height]
  let cells = concat $ zipWith zipRow [0..width] vals
  let minerals = filter (\(_, x) -> x > 0) cells
  let pits = map fst $ filter (\(_, x) -> x < 0) cells
  return $ M (width, height) minerals pits (computeSensorValues minerals cells)

computeSensorValues :: [(Point, MQty)] -> [(Point, SVal)] -> [(Point, SVal)]
computeSensorValues m c = foldr updateSensorValue c m

updateSensorValue :: (Point, MQty) -> [(Point, SVal)] -> [(Point, SVal)]
updateSensorValue ((x, y), q) = map update
  where
    update (p@(x', y'), q')
      | x == x' && y == y' = (p, q')
      | x == x' = (p, q `div` val y y' + q')
      | y == y' = (p, q `div` val x x' + q')
      | otherwise = (p, q')
    val x x' = 2 ^ abs (x - x')
