module Types
where

-- Size: (Width, Height)
type Size = (Int, Int)

-- Point: (X, Y)
type Point = (Int, Int)

-- Sensor value
type SVal = Int

-- Mineral quantity
type MQty = Int

-- Maze: represented as a structure, not quite efficient representation
data Maze
  = M
  { size :: Size -- size of maze, (width, height)
  , minerals :: [(Point, MQty)] -- list of all cells containing minerals
  , pits :: [Point] -- list of all pits (impassable tiles)
  , sensors :: [(Point, SVal)] -- for each point, the value of sensor
  } deriving (Eq, Show)

-- Cardinal directions.
data Cardinal = N | E | S | W deriving (Eq, Show, Ord, Enum)

-- Robot action: Nothing or Just Cardinal
type Action = Maybe Cardinal

-- A simulated robot.
data Robot a = R
  { position :: (Int, Int) -- position (private, robot doesn't know it)
  , points :: Int -- quantity of minerals gathered
  , memory :: a -- memory of robot
  } deriving (Eq)

-- Instance for Show for Robot (cannot show arbitrary memory type).
instance Show (Robot a) where
  show (R p ps s) = show ps ++ " @ " ++ show p
