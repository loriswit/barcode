if (typeof kotlin === 'undefined') {
  throw new Error("Error loading module 'barcode'. Its dependency 'kotlin' was not found. Please, check whether 'kotlin' is loaded prior to 'barcode'.");
}
var barcode = function (_, Kotlin) {
  'use strict';
  var addAll = Kotlin.kotlin.collections.addAll_ye1y7v$;
  var Unit = Kotlin.kotlin.Unit;
  var joinToString = Kotlin.kotlin.collections.joinToString_fmv235$;
  var Kind_OBJECT = Kotlin.Kind.OBJECT;
  var numberToInt = Kotlin.numberToInt;
  var throwCCE = Kotlin.throwCCE;
  var asList = Kotlin.org.w3c.dom.asList_kt9thq$;
  var removeSuffix = Kotlin.kotlin.text.removeSuffix_gsj5wt$;
  var toInt = Kotlin.kotlin.text.toInt_pdl1vz$;
  var Enum = Kotlin.kotlin.Enum;
  var Kind_CLASS = Kotlin.Kind.CLASS;
  var throwISE = Kotlin.throwISE;
  var replace = Kotlin.kotlin.text.replace_680rmw$;
  Action.prototype = Object.create(Enum.prototype);
  Action.prototype.constructor = Action;
  var ArrayList_init = Kotlin.kotlin.collections.ArrayList_init_ww73n8$;
  function Barcode() {
    Barcode_instance = this;
    this.packets_0 = ArrayList_init();
  }
  Barcode.prototype.addPacket_jg0ilm$ = function (packet) {
    this.packets_0.add_11rb$(packet);
  };
  Barcode.prototype.clear = function () {
    this.packets_0.clear();
  };
  var copyToArray = Kotlin.kotlin.collections.copyToArray;
  Barcode.prototype.serialize = function () {
    var bits = ArrayList_init();
    addAll(bits, (new Packet(Action$SYNC_getInstance())).serialize());
    var tmp$;
    tmp$ = this.packets_0.iterator();
    while (tmp$.hasNext()) {
      var element = tmp$.next();
      addAll(bits, element.serialize());
    }
    addAll(bits, (new Packet(Action$END_getInstance())).serialize());
    return copyToArray(bits);
  };
  Barcode.prototype.toString = function () {
    return joinToString(this.packets_0);
  };
  Barcode.$metadata$ = {
    kind: Kind_OBJECT,
    simpleName: 'Barcode',
    interfaces: []
  };
  var Barcode_instance = null;
  function Barcode_getInstance() {
    if (Barcode_instance === null) {
      new Barcode();
    }
    return Barcode_instance;
  }
  function Canvas() {
    Canvas_instance = this;
    this.barWidth_0 = 20.0;
    this.barHeight_0 = 600.0;
    var tmp$, tmp$_0;
    this.canvas_0 = Kotlin.isType(tmp$ = document.getElementsByTagName('canvas')[0], HTMLCanvasElement) ? tmp$ : throwCCE();
    this.scene_0 = Kotlin.isType(tmp$_0 = this.canvas_0.getContext('2d'), CanvasRenderingContext2D) ? tmp$_0 : throwCCE();
    this.canvas_0.height = 600;
  }
  Canvas.prototype.update = function () {
    var bits = Barcode_getInstance().serialize();
    this.canvas_0.width = numberToInt(bits.length * this.barWidth_0);
    for (var index = 0; index !== bits.length; ++index) {
      var bit = bits[index];
      this.scene_0.fillStyle = 'hsl(' + ((index / 6 | 0) * 150 | 0) + ', ' + (100 - (index % 2 * 50 | 0) | 0) + '%, ' + (75 - (index % 2 * 50 | 0) | 0) + '%)';
      this.scene_0.fillRect(index * this.barWidth_0, 0.0, this.barWidth_0, this.barHeight_0);
      this.scene_0.fillStyle = bit ? 'black' : 'white';
      this.scene_0.fillRect(index * this.barWidth_0, this.barWidth_0, this.barWidth_0, this.barHeight_0 - 2 * this.barWidth_0);
    }
  };
  Canvas.prototype.clear = function () {
    this.scene_0.clearRect(0.0, 0.0, this.canvas_0.width, this.canvas_0.height);
  };
  Canvas.prototype.dataURL = function () {
    return this.canvas_0.toDataURL();
  };
  Canvas.$metadata$ = {
    kind: Kind_OBJECT,
    simpleName: 'Canvas',
    interfaces: []
  };
  var Canvas_instance = null;
  function Canvas_getInstance() {
    if (Canvas_instance === null) {
      new Canvas();
    }
    return Canvas_instance;
  }
  function main$lambda(closure$row) {
    return function (it) {
      displayButtonRow(closure$row);
      return Unit;
    };
  }
  function main$lambda_0(closure$saveLink) {
    return function (it) {
      closure$saveLink.href = Canvas_getInstance().dataURL();
      return Unit;
    };
  }
  function main$lambda_1(it) {
    clearPackets();
    return Unit;
  }
  function main(args) {
    var tmp$, tmp$_0, tmp$_1, tmp$_2, tmp$_3;
    tmp$ = asList(document.querySelectorAll('#main-row button')).iterator();
    while (tmp$.hasNext()) {
      var node = tmp$.next();
      var button = Kotlin.isType(tmp$_0 = node, HTMLElement) ? tmp$_0 : throwCCE();
      var row = removeSuffix(button.id, '-button');
      (tmp$_1 = document.getElementById(button.id)) != null ? (tmp$_1.addEventListener('click', main$lambda(row)), Unit) : null;
    }
    listenAction('move-forward-button', 'move-units', Action$MOVE_FORWARD_getInstance());
    listenAction('move-backward-button', 'move-units', Action$MOVE_BACKWARD_getInstance());
    listenAction('rotate-left-button', 'rotate-degrees', Action$ROTATE_LEFT_getInstance());
    listenAction('rotate-right-button', 'rotate-degrees', Action$ROTATE_RIGHT_getInstance());
    listenAction('led-on-button', 'led-number', Action$LED_ON_getInstance());
    listenAction('led-off-button', 'led-number', Action$LED_OFF_getInstance());
    listenAction('wait-wait-button', 'wait-time', Action$WAIT_getInstance());
    var saveLink = Kotlin.isType(tmp$_2 = document.getElementById('save'), HTMLAnchorElement) ? tmp$_2 : throwCCE();
    saveLink.addEventListener('click', main$lambda_0(saveLink));
    (tmp$_3 = document.getElementById('reset')) != null ? (tmp$_3.addEventListener('click', main$lambda_1), Unit) : null;
    hideButtonRows();
    Canvas_getInstance().update();
  }
  function listenAction$lambda(closure$input, closure$action) {
    return function (it) {
      if (closure$input.reportValidity())
        registerPacket(closure$action, toInt(closure$input.value));
      return Unit;
    };
  }
  function listenAction(buttonID, valueID, action) {
    var tmp$;
    var button = document.getElementById(buttonID);
    var input = Kotlin.isType(tmp$ = document.getElementById(valueID), HTMLInputElement) ? tmp$ : throwCCE();
    button != null ? (button.addEventListener('click', listenAction$lambda(input, action)), Unit) : null;
  }
  function refresh() {
    var tmp$;
    Canvas_getInstance().clear();
    Canvas_getInstance().update();
    (tmp$ = document.getElementById('packets')) != null ? (tmp$.innerHTML = Barcode_getInstance().toString()) : null;
  }
  function registerPacket(action, value) {
    Barcode_getInstance().addPacket_jg0ilm$(new Packet(action, value));
    hideButtonRows();
    refresh();
  }
  function clearPackets() {
    Barcode_getInstance().clear();
    refresh();
  }
  function hideButtonRows() {
    var tmp$, tmp$_0, tmp$_1;
    tmp$ = asList(document.getElementsByClassName('button-row')).iterator();
    while (tmp$.hasNext()) {
      var row = tmp$.next();
      row.classList.add('hidden');
    }
    tmp$_0 = asList(document.querySelectorAll('#main-row button')).iterator();
    while (tmp$_0.hasNext()) {
      var node = tmp$_0.next();
      var button = Kotlin.isType(tmp$_1 = node, HTMLElement) ? tmp$_1 : throwCCE();
      button.classList.remove('selected');
    }
  }
  function displayButtonRow(rowID) {
    var tmp$, tmp$_0, tmp$_1, tmp$_2;
    hideButtonRows();
    (tmp$_0 = (tmp$ = document.getElementById(rowID + '-row')) != null ? tmp$.classList : null) != null ? (tmp$_0.remove('hidden'), Unit) : null;
    (tmp$_2 = (tmp$_1 = document.getElementById(rowID + '-button')) != null ? tmp$_1.classList : null) != null ? (tmp$_2.add('selected'), Unit) : null;
  }
  function Action(name, ordinal, id, description) {
    Enum.call(this);
    this.id = id;
    this.description = description;
    this.name$ = name;
    this.ordinal$ = ordinal;
  }
  function Action_initFields() {
    Action_initFields = function () {
    };
    Action$MOVE_FORWARD_instance = new Action('MOVE_FORWARD', 0, 0, 'move ? units forward');
    Action$MOVE_BACKWARD_instance = new Action('MOVE_BACKWARD', 1, 1, 'move ? units backward');
    Action$ROTATE_LEFT_instance = new Action('ROTATE_LEFT', 2, 2, 'rotate ?\xB0 left');
    Action$ROTATE_RIGHT_instance = new Action('ROTATE_RIGHT', 3, 3, 'rotate ?\xB0 right');
    Action$LED_ON_instance = new Action('LED_ON', 4, 4, 'turn LED ? on');
    Action$LED_OFF_instance = new Action('LED_OFF', 5, 5, 'turn LED ? off');
    Action$WAIT_instance = new Action('WAIT', 6, 6, 'wait for ? seconds');
    Action$END_instance = new Action('END', 7, 7, 'end of stream');
    Action$SYNC_instance = new Action('SYNC', 8, 8, 'synchronisation');
  }
  var Action$MOVE_FORWARD_instance;
  function Action$MOVE_FORWARD_getInstance() {
    Action_initFields();
    return Action$MOVE_FORWARD_instance;
  }
  var Action$MOVE_BACKWARD_instance;
  function Action$MOVE_BACKWARD_getInstance() {
    Action_initFields();
    return Action$MOVE_BACKWARD_instance;
  }
  var Action$ROTATE_LEFT_instance;
  function Action$ROTATE_LEFT_getInstance() {
    Action_initFields();
    return Action$ROTATE_LEFT_instance;
  }
  var Action$ROTATE_RIGHT_instance;
  function Action$ROTATE_RIGHT_getInstance() {
    Action_initFields();
    return Action$ROTATE_RIGHT_instance;
  }
  var Action$LED_ON_instance;
  function Action$LED_ON_getInstance() {
    Action_initFields();
    return Action$LED_ON_instance;
  }
  var Action$LED_OFF_instance;
  function Action$LED_OFF_getInstance() {
    Action_initFields();
    return Action$LED_OFF_instance;
  }
  var Action$WAIT_instance;
  function Action$WAIT_getInstance() {
    Action_initFields();
    return Action$WAIT_instance;
  }
  var Action$END_instance;
  function Action$END_getInstance() {
    Action_initFields();
    return Action$END_instance;
  }
  var Action$SYNC_instance;
  function Action$SYNC_getInstance() {
    Action_initFields();
    return Action$SYNC_instance;
  }
  Action.$metadata$ = {
    kind: Kind_CLASS,
    simpleName: 'Action',
    interfaces: [Enum]
  };
  function Action$values() {
    return [Action$MOVE_FORWARD_getInstance(), Action$MOVE_BACKWARD_getInstance(), Action$ROTATE_LEFT_getInstance(), Action$ROTATE_RIGHT_getInstance(), Action$LED_ON_getInstance(), Action$LED_OFF_getInstance(), Action$WAIT_getInstance(), Action$END_getInstance(), Action$SYNC_getInstance()];
  }
  Action.values = Action$values;
  function Action$valueOf(name) {
    switch (name) {
      case 'MOVE_FORWARD':
        return Action$MOVE_FORWARD_getInstance();
      case 'MOVE_BACKWARD':
        return Action$MOVE_BACKWARD_getInstance();
      case 'ROTATE_LEFT':
        return Action$ROTATE_LEFT_getInstance();
      case 'ROTATE_RIGHT':
        return Action$ROTATE_RIGHT_getInstance();
      case 'LED_ON':
        return Action$LED_ON_getInstance();
      case 'LED_OFF':
        return Action$LED_OFF_getInstance();
      case 'WAIT':
        return Action$WAIT_getInstance();
      case 'END':
        return Action$END_getInstance();
      case 'SYNC':
        return Action$SYNC_getInstance();
      default:throwISE('No enum constant barcode.Action.' + name);
    }
  }
  Action.valueOf_61zpoe$ = Action$valueOf;
  function Packet(action, value) {
    if (value === void 0)
      value = 1;
    this.action_0 = action;
    this.value_0 = value;
  }
  var Array_0 = Array;
  Packet.prototype.serialize = function () {
    var tmp$;
    switch (this.action_0.name) {
      case 'MOVE_FORWARD':
      case 'MOVE_BACKWARD':
      case 'WAIT':
        tmp$ = this.value_0 - 1 | 0;
        break;
      case 'ROTATE_LEFT':
      case 'ROTATE_RIGHT':
        tmp$ = (this.value_0 / 45 | 0) - 1 | 0;
        break;
      case 'LED_ON':
      case 'LED_OFF':
        tmp$ = this.value_0;
        break;
      case 'END':
        return [true, true, true];
      case 'SYNC':
        return [true, false, true, false, true, false];
      default:tmp$ = Kotlin.noWhenBranchMatched();
        break;
    }
    var code = tmp$;
    var array = Array_0(6);
    var tmp$_0;
    tmp$_0 = array.length - 1 | 0;
    for (var i = 0; i <= tmp$_0; i++) {
      array[i] = false;
    }
    var bits = array;
    for (var index = 0; index <= 2; index++) {
      bits[index] = (this.action_0.id >> index & 1) === 1;
      bits[index + 3 | 0] = (code >> index & 1) === 1;
    }
    return bits;
  };
  Packet.prototype.toString = function () {
    return replace(this.action_0.description, '?', this.value_0.toString());
  };
  Packet.$metadata$ = {
    kind: Kind_CLASS,
    simpleName: 'Packet',
    interfaces: []
  };
  Packet.prototype.component1_0 = function () {
    return this.action_0;
  };
  Packet.prototype.component2_0 = function () {
    return this.value_0;
  };
  Packet.prototype.copy_vnk9l6$ = function (action, value) {
    return new Packet(action === void 0 ? this.action_0 : action, value === void 0 ? this.value_0 : value);
  };
  Packet.prototype.hashCode = function () {
    var result = 0;
    result = result * 31 + Kotlin.hashCode(this.action_0) | 0;
    result = result * 31 + Kotlin.hashCode(this.value_0) | 0;
    return result;
  };
  Packet.prototype.equals = function (other) {
    return this === other || (other !== null && (typeof other === 'object' && (Object.getPrototypeOf(this) === Object.getPrototypeOf(other) && (Kotlin.equals(this.action_0, other.action_0) && Kotlin.equals(this.value_0, other.value_0)))));
  };
  var package$barcode = _.barcode || (_.barcode = {});
  Object.defineProperty(package$barcode, 'Barcode', {
    get: Barcode_getInstance
  });
  Object.defineProperty(package$barcode, 'Canvas', {
    get: Canvas_getInstance
  });
  package$barcode.main_kand9s$ = main;
  package$barcode.listenAction_aqa0dc$ = listenAction;
  package$barcode.refresh = refresh;
  package$barcode.registerPacket_vnk9l6$ = registerPacket;
  package$barcode.clearPackets = clearPackets;
  package$barcode.hideButtonRows = hideButtonRows;
  package$barcode.displayButtonRow_61zpoe$ = displayButtonRow;
  Object.defineProperty(Action, 'MOVE_FORWARD', {
    get: Action$MOVE_FORWARD_getInstance
  });
  Object.defineProperty(Action, 'MOVE_BACKWARD', {
    get: Action$MOVE_BACKWARD_getInstance
  });
  Object.defineProperty(Action, 'ROTATE_LEFT', {
    get: Action$ROTATE_LEFT_getInstance
  });
  Object.defineProperty(Action, 'ROTATE_RIGHT', {
    get: Action$ROTATE_RIGHT_getInstance
  });
  Object.defineProperty(Action, 'LED_ON', {
    get: Action$LED_ON_getInstance
  });
  Object.defineProperty(Action, 'LED_OFF', {
    get: Action$LED_OFF_getInstance
  });
  Object.defineProperty(Action, 'WAIT', {
    get: Action$WAIT_getInstance
  });
  Object.defineProperty(Action, 'END', {
    get: Action$END_getInstance
  });
  Object.defineProperty(Action, 'SYNC', {
    get: Action$SYNC_getInstance
  });
  package$barcode.Action = Action;
  package$barcode.Packet = Packet;
  main([]);
  Kotlin.defineModule('barcode', _);
  return _;
}(typeof barcode === 'undefined' ? {} : barcode, kotlin);

//# sourceMappingURL=barcode.js.map
