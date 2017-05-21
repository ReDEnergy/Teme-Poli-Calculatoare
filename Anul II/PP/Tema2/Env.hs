module Env
where

import GUI
import Robot
import Types

-- Main function of the environment (also construct GUI)
envMain :: Maze -> IO ()
envMain m = buildGUI step m $ R (0, 0) 0 (startRobot $ size m)

{-
Action to be called when the robot needs to do a new step. Updates the
environment as well.
-}
-- step :: Maze -> Robot a -> Maybe (Maze, Robot a)
step m r
  | act `elem` map Just blockedCells = Nothing -- robot dies
  | otherwise = Just (m { minerals = remaining, sensors = s' }, R p' ps mem)
  where
    p = position r
    s = sensors m
    ms = minerals m
    blockedCells = getBlockedDirections m p
    sensorVal = snd . head . filter ((== p) . fst) $ s
    (act, mem) = perceiveAndAct sensorVal blockedCells (memory r)
    p' = maybe p (increase p) act
    mineralsFound = safeHead . filter ((== p') . fst) $ ms
    ps = let p0 = points r in maybe p0 ((+ p0) . snd) mineralsFound
    remaining = maybe ms (\x -> filter (/= x) ms) mineralsFound
    s' = maybe s (updateSensors s) mineralsFound
    increase (x, y) N = (x - 1, y)
    increase (x, y) S = (x + 1, y)
    increase (x, y) E = (x, y + 1)
    increase (x, y) W = (x, y - 1)
    safeHead l = if l == [] then Nothing else Just $ head l

updateSensors :: [(Point, SVal)] -> (Point, MQty) -> [(Point, SVal)]
updateSensors s ((x, y), q) = map update s
  where
    update (p@(x', y'), q')
      | x == x' && y == y' = (p, q' - q)
      | x == x' = (p, q' - q `div` val y y')
      | y == y' = (p, q' - q `div` val x x')
      | otherwise = (p, q')
    val x x' = 2 ^ abs (x - x')

{-
Return list of directions the robot is not allowed to go to.
-}
getBlockedDirections :: Maze -> Point -> [Cardinal]
getBlockedDirections m p = blocked p ++ concatMap (relativeTo p) (pits m)
  where
    relativeTo (x, y) (x', y')
      | x == x' + 1 && y == y' = [N]
      | x == x' - 1 && y == y' = [S]
      | x == x' && y == y' + 1 = [W]
      | x == x' && y == y' - 1 = [E]
      | otherwise = []
    (sx, sy) = size m
    blocked (x, y)
      | x == 0 && y == 0 = [N, W]
      | x == sx - 1 && y == sy - 1 = [S, E]
      | x == 0 && y == sy - 1 = [N, E]
      | x == sx - 1 && y == 0 = [S, W]
      | x == 0 = [N]
      | y == 0 = [W]
      | y == sy - 1 = [E]
      | x == sx - 1 = [S]
      | otherwise = []
