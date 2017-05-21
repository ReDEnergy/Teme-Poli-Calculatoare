module GUI
where

import Control.Monad
import Data.IORef
import Graphics.Rendering.Cairo
import Graphics.UI.Gtk hiding (Point, Size)

import Types

gN = 8
gN2 = 4
gIMGDIR = "res/"
gEMPTY = gIMGDIR ++ "empty.png"
gVOID = gIMGDIR ++ "void.png"
gROBOT = gIMGDIR ++ "robot.png"
gMINERAL = gIMGDIR ++ "mineral.png"
gPIT = gIMGDIR ++ "pit.png"

type StepF a =  Maze -> Robot a -> Maybe (Maze, Robot a)
type IORT a = (Maze, Robot a, Int, StepF a)

-- Builds the entire GUI for the application
-- buildGUI :: StepF a -> Maze -> Robot a -> IO ()
buildGUI step m r = do
  initGUI
  window <- windowNew
  window `on` deleteEvent $ liftIO mainQuit >> return False
  let f = windowSetGeometryHints window (Nothing :: Maybe Window)
  f (Just (800, 600)) (Just (800, 600)) Nothing Nothing Nothing
  set window [ windowTitle := "Robot in a Mine" ]
  ref <- newIORef (m, r, 0, step)
  (imgs, stl, qtyl, sensl) <- populateWindow window ref
  updateWorld ref imgs stl qtyl sensl
  widgetShowAll window
  mainGUI

-- onStep :: Window -> IORef (IORT a) -> [(Point, Image)] -> Label -> Label -> Label -> IO ()
onStep w ref imgs stl qtyl sensl = do
  (m, r, s, step) <- readIORef ref -- maze, robot, step, nextStep
  case step m r of
    Nothing -> kill w ref
    Just (m', r') -> writeIORef ref (m', r', s + 1, step)
  updateWorld ref imgs stl qtyl sensl

-- kill :: Window -> IORef (IORT a) -> IO ()
kill wnd ref = do
  (m, r, s, step) <- readIORef ref
  let f = messageDialogNew (Just wnd) [] MessageError
  w <- f ButtonsOk "Robot has died"
  dialogRun w
  widgetDestroy w
  writeIORef ref (m, r, s, \m r -> Nothing)

-- updateWorld :: IORef (IORT a) -> [(Point, Image)] -> Label -> Label -> Label -> IO ()
updateWorld ref imgs stl qtyl sensl = do
  (m, r, s, _) <- readIORef ref -- maze, robot, step, nextStep
  let p = position r
  let sensorVal = snd . head . filter ((== p) . fst) $ sensors m
  mapM_ (updateTile (size m) p (map fst $ minerals m) (pits m) (off p)) imgs
  updateLabel stl s
  updateLabel qtyl (points r)
  updateLabel sensl sensorVal

updateTile :: Size -> Point -> [Point] -> [Point] -> Point -> (Point, Image) -> IO ()
updateTile (sx, sy) (rx, ry) mins pits (offx, offy) ((x_, y_), i)
  | (x, y) == (rx, ry) = i `imageSetFromFile` gROBOT
  | (x, y) `elem` mins = i `imageSetFromFile` gMINERAL
  | (x, y) `elem` pits = i `imageSetFromFile` gPIT
  | x < 0 = i `imageSetFromFile` gVOID
  | y < 0 = i `imageSetFromFile` gVOID
  | x >= sx = i `imageSetFromFile` gVOID
  | y >= sy = i `imageSetFromFile` gVOID
  | otherwise = i `imageSetFromFile` gEMPTY
  where
    x = y_ + offy
    y = x_ + offx

off :: Point -> Point
off (x, y) = (y - gN2, x - gN2)

-- populateWindow :: Window -> IORef (IORT a) -> IO ([(Point, Image)], Label, Label, Label)
populateWindow w ref = do
  vBox <- vBoxNew False 10
  w `containerAdd` vBox
  images <- buildMatrix vBox
  (step, qty, sensor) <- buildToolbar w vBox ref images
  return (images, step, qty, sensor)

buildMatrix :: VBox -> IO [(Point, Image)]
buildMatrix b = do
  h <- hBoxNew False 10
  table <- tableNew gN gN True
  let l = [(x, y) | x <- [0 .. gN -1], y <- [0 .. gN - 1]]
  fields <- mapM (initTile table) l
  boxPackStart h table PackRepel 5
  boxPackEnd b h PackGrow 10
  return fields

initTile :: Table -> (Int, Int) -> IO (Point, Image)
initTile table (x, y) = do
  img <- imageNewFromFile gVOID
  tableAttachDefaults table img x (x + 1) y (y + 1)
  return ((x, y), img)

-- buildToolbar :: Window -> VBox -> IORef (IORT a) -> [(Point, Image)] -> IO (Label, Label, Label)
buildToolbar w b ref imgs = do
  tb <- toolbarNew
  boxPackStart b tb PackNatural 10
  bStep <- addBtnToToolbar tb stockGoForward "Step"
  addSeparator tb
  step <- buildLabelText tb "Current step: " 0
  addSeparator tb
  qty <- buildLabelText tb "Quantity mined: " 0
  addSeparator tb
  sensor <- buildLabelText tb "Sensor: " 0
  bStep `onToolButtonClicked` onStep w ref imgs step qty sensor
  return (step, qty, sensor)

addBtnToToolbar :: Toolbar -> StockId -> String -> IO ToolButton
addBtnToToolbar tb stock label = do
  b <- toolButtonNewFromStock stock
  toolbarInsertNext tb b
  return b

addSeparator :: Toolbar -> IO ()
addSeparator tb = do
  s <- separatorToolItemNew
  toolbarInsertNext tb s

toolbarInsertNext :: (ToolItemClass a) => Toolbar -> a -> IO ()
toolbarInsertNext tb item = toolbarInsert tb item $ -1

buildLabelText :: Toolbar -> String -> Int -> IO Label
buildLabelText tb txt val = do
  ti <- toolItemNew
  b <- hBoxNew False 5
  ti `containerAdd` b
  l1 <- labelNew $ Just txt
  boxPackStart b l1 PackNatural 10
  l2 <- labelNew $ Just $ show val
  boxPackStart b l2 PackNatural 10
  toolbarInsertNext tb ti
  return l2

updateLabel :: Label -> Int -> IO ()
updateLabel lbl val = lbl `labelSetText` show val
