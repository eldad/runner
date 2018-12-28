/*
     Color Blocks
 */

let colors = [|"#00A878", "#D8F1A0", "#F3C178", "#FE5E41"|];

let blocksize = "100px";

let block_hightlight_animation =
  Glamor.(
    keyframes([("0%", [filter("grayscale(0)")]), ("50%", [filter("grayscale(1)")]), ("100%", [filter("grayscale(0)")])])
  );

let blockClassName = (is_highlighted: bool, bgcolor: string) =>
  is_highlighted ?
    Glamor.(
      css([
        backgroundColor(bgcolor),
        width(blocksize),
        height(blocksize),
        animationName(block_hightlight_animation),
        animationDuration("1s"),
      ])
    ) :
    Glamor.(css([backgroundColor(bgcolor), width(blocksize), height(blocksize)]));

let component = ReasonReact.statelessComponent("ColorBlocks");

let make = (~highlight: option(int)=?, ~onBlockClick, _children) => {
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
               key={i |> string_of_int}
               className={blockClassName(is_highlighted, color)}
               onClick={_e => onBlockClick(i)}
             />;
           })
        |> ReasonReact.array
      }
    </div>,
};
