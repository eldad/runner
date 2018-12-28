/*
     Color Blocks
 */

[%bs.raw {|require('./ColorBlocks.css')|}];

let colors = [|"#00A878", "#D8F1A0", "#F3C178", "#FE5E41"|];

let blocksize = "100px";

let blockstyle = (is_highlighted: bool, color: string) =>
  is_highlighted ?
    ReactDOMRe.Style.make(
      ~backgroundColor=color,
      ~width=blocksize,
      ~height=blocksize,
      ~animation="colorblocks_highlight 1s",
      (),
    ) :
    ReactDOMRe.Style.make(~backgroundColor=color, ~width=blocksize, ~height=blocksize, ());

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

             <button key={i |> string_of_int} style={blockstyle(is_highlighted, color)} onClick={_e => onBlockClick(i)} />;
           })
        |> ReasonReact.array
      }
    </div>,
};
