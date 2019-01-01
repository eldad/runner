/*
   App View
 */

let debug = false;

Random.self_init();

module CanvasData = {
  let bg_image = CanvasBackgroundImage.make(~src="media/bgimage.png", ~width=1600, ~height=316, ());
};

/* Use polymorphic variants, because they get converted to strings */
[@bs.deriving jsConverter]
type stage = [ | `Idle | `Play | `Input];

type action =
  | Play
  | Tick
  | CanvasClick(int, int);

type state = {
  color_highlighted: option(int),
  stage,
  bgscroll: float,
  clicks: int,
  x: int,
  y: int,
  last_tick: option(float),
};

let handleTick: (state, float) => state =
  (state, delta_t) => delta_t > 1.0 ? state : {...state, bgscroll: state.bgscroll +. delta_t *. (state.clicks |> float_of_int)};

let initialState = () => {color_highlighted: None, stage: `Idle, x: 0, y: 0, clicks: 100, bgscroll: 1550., last_tick: None};

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  ...component,
  initialState,
  didMount: self => {
    let intervalId = Js.Global.setInterval(() => self.send(Tick), 0);
    self.onUnmount(() => Js.Global.clearInterval(intervalId));
  },
  reducer: (action, state) =>
    switch (action) {
    | Tick =>
      let now = Js.Date.now() /. 1000.0;
      switch (state.last_tick) {
      | None => ReasonReact.Update({...state, last_tick: Some(now)})
      | Some(last_tick) =>
        let delta_t = now -. last_tick;
        ReasonReact.Update({...state->handleTick(delta_t), last_tick: Some(now)});
      };
    | Play when state.stage == `Idle =>
      ReasonReact.UpdateWithSideEffects(
        {...state, stage: `Play},
        (_self => Dom_html.requestFullscreenElement("canvas-") |> ignore),
      )
    | Play => ReasonReact.NoUpdate
    | CanvasClick(x, y) => ReasonReact.Update({...state, clicks: state.clicks + 1, x, y})
    },
  didUpdate: oldAndNewSelf => {
    let self = oldAndNewSelf.newSelf;
    switch (Dom_html.getCanvas2DContext("canvas")) {
    | Some(context) =>
      Dom_html.requestAnimationFrame(_f =>
        CanvasRender.render(
          ~context,
          ~width=640,
          ~height=320,
          ~bg_image=CanvasData.bg_image,
          ~bgscroll=self.state.bgscroll |> int_of_float,
          (),
        )
      )
      |> ignore
    | None => ()
    };
  },
  render: self => {
    let debug_info =
      debug ?
        <div className=Glamor.(css([backgroundColor("#000")]))> {self.state.stage |> stageToJs |> ReasonReact.string} </div> :
        ReasonReact.null;

    <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
      <div className=Glamor.(css([backgroundColor("#333")]))>
        {Printf.sprintf("Clicks: %d, x: %d, y: %d", self.state.clicks, self.state.x, self.state.y) |> ReasonReact.string}
      </div>
      debug_info
      {
        switch (self.state.stage) {
        | `Idle => <div> <button onClick=(_e => self.send(Play))> {"Play" |> ReasonReact.string} </button> </div>
        | _ => ReasonReact.null
        }
      }
      <canvas
        id="canvas"
        width="640"
        height="320"
        onClick={e => self.send(CanvasClick(e |> ReactEvent.Mouse.clientX, e |> ReactEvent.Mouse.clientY))}
      />
    </div>;
  },
};
