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

let render_banner = (context, viewport_width, text) => {
  let margin = 20;
  let y = 100;
  let fontsize = 48;

  context->(Dom_html.setFont("48px 'Press Start 2P'"));
  let text_width = context->(Dom_html.measureTextWidth(text));
  let text_xpos = (viewport_width - text_width) / 2;

  context->Dom_html.fillStyle("#0007");
  context->Dom_html.fillRect(text_xpos - margin, y - margin - fontsize, text_width + margin * 2, fontsize + margin * 2);

  context->Dom_html.fillStyle("#fff");
  context->(Dom_html.fillText(text, text_xpos, y));
};

let render_score = (context, points: int) => {
  let margin = 10;
  let y = 20;
  let fontsize = 16;

  let text = Printf.sprintf("Points: %d", points);

  context->(Dom_html.setFont("16px 'Press Start 2P'"));
  let text_width = context->(Dom_html.measureTextWidth(text));
  let text_xpos = 10;

  context->Dom_html.fillStyle("#0003");
  context->Dom_html.fillRect(text_xpos - margin, y - margin - fontsize, text_width + margin * 2, fontsize + margin * 2);

  context->Dom_html.fillStyle("#fff");
  context->(Dom_html.fillText(text, text_xpos, y));
};

let render = (~width, ~height, ~context: Dom_html.context, ~data: GameState.t, ()) => {
  context->(Dom_html.clearRect(0, 0, width, height));

  let bgscroll = data.distance |> int_of_float;

  /* Background */

  context->(CanvasBackgroundImage.render(CanvasData.bg, width, height, bgscroll->parallax(6)));
  context->(CanvasBackgroundImage.render(CanvasData.distantstreet, width, height, bgscroll->parallax(5)));
  context->(CanvasBackgroundImage.render(CanvasData.fgstreet, width, height, bgscroll->parallax(2)));
  context->(CanvasBackgroundImage.render(CanvasData.powerlines, width, height, bgscroll->parallax(1)));
  context->(CanvasBackgroundImage.render(CanvasData.pavement, width, height, bgscroll->parallax(0)));

  /* Player */

  let player_sprite = data.player_y == 0. ? CanvasData.greenbob : CanvasData.greenbobjump;

  let player_frame =
    switch (data.state) {
    | Run => data.time *. 10. |> Js.Math.floor
    | _ => 5
    };

  let player_y = CanvasData.pavement_height + (data.player_y |> Js.Math.floor);
  context->(CanvasSprite.render(~viewport_h=height, player_sprite, 10, player_y, player_frame));

  /* Obstacles */

  data.obstacles
  |> Array.iter(pos => {
       let relative_pos = pos -. data.distance |> Js.Math.floor;
       if (relative_pos + CanvasData.crate.width > 0 && relative_pos < width) {
         context->(CanvasSprite.render(~viewport_h=height, CanvasData.crate, relative_pos, CanvasData.pavement_height, 0));
       };
     });

  /* Banner */

  switch (data.state) {
  | Idle => context->render_banner(width, "Ready?")
  | Run when data.time < 2.0 => context->render_banner(width, "Run!")
  | GameOver(_) => context->render_banner(width, "Game Over")
  | _ => ()
  };

  /* Score */

  switch (data.state) {
  | Run when data.time > 3.0 => context->render_score(data.score)
  | GameOver(_) => context->render_score(data.score)
  | _ => ()
  };

  ();
};
