/*
     Canvas
 */

let parallax = (offset: int, plane_index: int) =>
  /* index: 0 = foreground (no transformation). index increases with "distance" */
  plane_index <= 0 ?
    offset :
    {
      let lagfactor = plane_index * plane_index |> float_of_int;
      (offset |> float_of_int) /. lagfactor |> Js.Math.floor;
    };

let render = (~width, ~height, ~context: Dom_html.context, ~data: GameState.t, ()) => {
  context->(Dom_html.clearRect(0, 0, width, height));

  let bgscroll = data.distance |> int_of_float;

  context->(CanvasBackgroundImage.render(CanvasData.bg, width, height, bgscroll->parallax(6)));
  context->(CanvasBackgroundImage.render(CanvasData.distantstreet, width, height, bgscroll->parallax(5)));
  context->(CanvasBackgroundImage.render(CanvasData.fgstreet, width, height, bgscroll->parallax(2)));
  context->(CanvasBackgroundImage.render(CanvasData.powerlines, width, height, bgscroll->parallax(1)));
  context->(CanvasBackgroundImage.render(CanvasData.pavement, width, height, bgscroll->parallax(0)));

  let player_sprite = data.player_y == 0. ? CanvasData.greenbob : CanvasData.greenbobjump;

  let player_frame = data.time *. 10. |> Js.Math.floor;
  let player_y = CanvasData.pavement_height + (data.player_y |> Js.Math.floor);
  context->(CanvasSprite.render(~viewport_h=height, player_sprite, 10, player_y, player_frame));

  data.obstacles
  |> Array.iter(pos => {
       let relative_pos = (pos -. data.distance) |> Js.Math.floor;
       if (relative_pos > 0 && relative_pos < width + CanvasData.crate.width ) {
         context->(CanvasSprite.render(~viewport_h=height, CanvasData.crate, relative_pos - CanvasData.crate.width, CanvasData.pavement_height, 0));
       };
     });

  ();
};
