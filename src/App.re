/*
  App View
*/

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => {
    ReasonReact.null
  },
};
