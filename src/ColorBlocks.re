/*
     Color Blocks
 */

let colors = [|"#00A878", "#D8F1A0", "#F3C178", "#FE5E41"|];

let blocksize = "75px";

let block_hightlight_animation =
  Glamor.(
    keyframes([("0%", [filter("opacity(1)")]), ("50%", [filter("opacity(0)")]), ("100%", [filter("opacity(1)")])])
  );

let blockClassName = (is_highlighted: bool, bgcolor: string) => {
  let base_style = Glamor.(css([backgroundColor(bgcolor), width(blocksize), height(blocksize)]));

  is_highlighted ?
    Glamor.(
      merge([
        base_style,
        css([animationName(block_hightlight_animation), animationDuration("0.5s"), animationTimingFunction("ease-in-out")]),
      ])
    ) :
    base_style;
};

let component = ReasonReact.statelessComponent("ColorBlocks");

let make = (~highlight: option(int)=?, ~disabled=true, ~onBlockClick=?, _children) => {
  ...component,
  render: _self =>
    <div>
      {
        colors
        |> Array.mapi((i, color) => {
             let is_highlighted =
               switch (highlight) {
               | None => false
               | Some(highlighted_index) => highlighted_index == i
               };

             <button
               disabled
               key={i |> string_of_int}
               className={blockClassName(is_highlighted, color)}
               onClick=?{
                 onBlockClick
                 |> Js.Option.map((. f) => {
                      let fn = _e => f(i);
                      fn; /* Syntax issue? */
                    })
               }
             />;
           })
        |> ReasonReact.array
      }
    </div>,
};
