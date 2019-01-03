/*
   Canvas Data
 */

let bg = CanvasBackgroundImage.make(~src="media/citymega-bg.png", ~width=1600, ~height=316, ());
let pavement = CanvasBackgroundImage.(make(~src="media/citymega-pavement.png", ~width=192, ~height=15, ~align=Bottom, ()));
let distantstreet =
  CanvasBackgroundImage.(make(~src="media/citymega-distantstreet.png", ~width=1600, ~height=268, ~align=Bottom, ()));
let powerlines = CanvasBackgroundImage.(make(~src="media/citymega-powerlines.png", ~width=1600, ~height=111, ~align=Bottom, ()));
let fgstreet = CanvasBackgroundImage.(make(~src="media/citymega-fgstreet.png", ~width=1600, ~height=242, ~align=Bottom, ()));

let pavement_height = 15;

let greenbob = CanvasSprite.make(~src="media/citymega-greenbob.png", ~width=32, ~height=32, ~frames=6, ());
let greenbobjump = CanvasSprite.make(~src="media/citymega-greenbob-jump.png", ~width=32, ~height=32, ~frames=2, ());
