/*
  App View
*/

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => {
    <div
      style=ReactDOMRe.Style.make(~display="flex",
      ~justifyContent="center",
      ())
    >
      <ColorBlocks />
    </div>
  },
};
