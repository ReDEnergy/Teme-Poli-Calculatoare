module Robot
where

import Types
import System.Random

{-
When the robot enters the mine it receives as input the size of the mine (it
is always placed at (0, 0)). This function should return the initial memory
element of the robot.
-}
-- startRobot :: Size -> a
startRobot size = ( (0,0), [], []);

{-
At each time step the robot sends a light beam in all 4 cardinal directions,
receives the reflected rays and computes their intensity (the first argument
of the function).

The robot sees nearby pits. The second argument of this function is the list
of neighbouring pits near the robot (if empty, there are no pits).

Taking into account the memory of the robot (third argument of the function),
it must return a tuple containing a new cardinal direction to go to and a new
memory element.

If the cardinal direction chosen goes to a pit or an wall the robot is
destroyed. If the new cell contains minerals they are immediately collected.
-}
-- perceiveAndAct :: SVal -> [Cardinal] -> a -> (Action, a)

newPos :: ((Int, Int), Int, Int) -> (Int, Int)
newPos ((a,b), x, y) = (a + x, b + y)

newCPos pos dir =
	if (dir == N)		then nposN
	else if (dir == S)	then nposS
	else if (dir == E)	then nposE
	else nposW
	where
		nposN = newPos(pos,-1, 0)
		nposS = newPos(pos, 1, 0)
		nposE = newPos(pos, 0, 1)
		nposW = newPos(pos, 0,-1)

perceiveAndAct s cs (pos, last, v) = 
	if (notElem E cs) && (notElem nposE v) 		then (Just E, (nposE, W:last, pos:v))
	else if (notElem W cs) && (notElem nposW v) then (Just W, (nposW, E:last, pos:v))
	else if (notElem N cs) && (notElem nposN v) then (Just N, (nposN, S:last, pos:v))
	else if (notElem S cs) && (notElem nposS v) then (Just S, (nposS, N:last, pos:v))
	else if (length last == 1) then (Just fLast, (nLast, newlast, []))
	else (Just fLast, (nLast, newlast, pos:v))
		where
			nposN = newPos(pos,-1, 0)
			nposS = newPos(pos, 1, 0)
			nposE = newPos(pos, 0, 1)
			nposW = newPos(pos, 0,-1)
			fLast = head last
			newlast = tail last
			nLast = newCPos pos fLast


