/*
   App View
 */

let debug = false;

let canvas_width = 640;
let canvas_height = 320;

Random.self_init();

type action =
  | Start
  | Fullscreen
  | Tick
  | CanvasClick(int, int);

type state = {
  start: bool,
  bgscroll: float,
  velocity: int,
  x: int,
  y: int,
  last_tick: option(float),
};

let handleTick: (state, float) => state =
  (state, delta_t) =>
    delta_t > 1.0 ? state : {...state, bgscroll: state.bgscroll +. delta_t *. (state.velocity |> float_of_int)};

let initialState = () => {start: false, x: 0, y: 0, velocity: 0, bgscroll: 0., last_tick: None};

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
    | Start => ReasonReact.Update({...state, start: true})
    | Tick when state.start =>
      let now = Js.Date.now() /. 1000.0;
      switch (state.last_tick) {
      | None => ReasonReact.Update({...state, last_tick: Some(now)})
      | Some(last_tick) =>
        let delta_t = now -. last_tick;
        ReasonReact.Update({...state->handleTick(delta_t), last_tick: Some(now)});
      };
    | Tick => ReasonReact.NoUpdate
    | Fullscreen => ReasonReact.SideEffects((_self => Dom_html.requestFullscreenElement("canvas") |> ignore))
    | CanvasClick(x, y) =>
      /* velocity -100..100 */
      let velocity = 200 * (x - canvas_width / 2) / canvas_width;
      ReasonReact.Update({...state, velocity, x, y});
    },
  didUpdate: oldAndNewSelf => {
    let self = oldAndNewSelf.newSelf;
    switch (Dom_html.getCanvas2DContext("canvas")) {
    | Some(context) =>
      Dom_html.requestAnimationFrame(_f =>
        CanvasRender.render(~context, ~width=640, ~height=320, ~bgscroll=self.state.bgscroll |> Js.Math.floor, ())
      )
      |> ignore
    | None => ()
    };
  },
  render: self =>
    <div className=Glamor.(css([display("flex"), alignItems("center"), flexDirection("column")]))>
      <div className=Glamor.(css([backgroundColor("#333")]))>
        {
          Printf.sprintf(
            "Scroll: %f, Velocity: %d, x: %d, y: %d",
            self.state.bgscroll,
            self.state.velocity,
            self.state.x,
            self.state.y,
          )
          |> ReasonReact.string
        }
      </div>
      <div>
        <button onClick={_e => self.send(Fullscreen)}> {"Fullscreen" |> ReasonReact.string} </button>
        <button onClick={_e => self.send(Start)}> {"Start" |> ReasonReact.string} </button>
      </div>
      <canvas
        id="canvas"
        width={canvas_width |> string_of_int}
        height={canvas_height |> string_of_int}
        onClick={
          e => {
            let ne = e |> ReactEvent.Mouse.nativeEvent;
            self.send(CanvasClick(ne##offsetX, ne##offsetY));
          }
        }
      />
    </div>,
};
