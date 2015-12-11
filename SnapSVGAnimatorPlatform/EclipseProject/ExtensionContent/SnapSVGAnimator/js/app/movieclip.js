
var MovieClip = function (parentMC, commandTimeline, resourceManager, objectID, name, placeAfter, transform) {
    var i,
        matrix,
        transformData,
        transformArray,
        afterEl,
        parentEl = parentMC.type == 'svg' ? parentMC : parentMC.el;  //parent is stage if svg

    if (objectID) {
      this.id = objectID;
    }

    if (name) {
      this.name = name;
      parentMC[this.name] = this;
    }

    this.el = parentEl.g();
    this.el.attr({'class': 'movieclip', 'token': this.id});
    this.transform = transform;

    this.m_timeline = commandTimeline;
    this.m_currentFrameNo = 0;
    this.m_frameCount = this.m_timeline.frameCount;

    this._scripts = {};
    this.children = [];
    this.isMask = false;
    this.isMasked = false;
    this.mask = null;
    this.maskElement = null;
    this.maskTill = null;
    this.loops = true;
    this.playing = true;
    this.resourceManager = resourceManager;
    this.commandList = [];

    if(this.transform !== undefined)
    {
        transformData = this.transform;
        transformArray = transformData.split(",");
        matrix = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
        this.el.transform(matrix);
    }

    if (placeAfter && parseInt(placeAfter) !== 0) {
        afterMC = parentMC.getChildById(parseInt(placeAfter));

        if (afterMC.isMasked) {  //if masked add outside mask group
            afterMC.el.parent().before(this.el);
        } else {
            afterMC.el.before(this.el);
        }

    } else {
        parentEl.add(this.el);
    }
};

//manage children methods
MovieClip.prototype.getChildById = function (id) {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            return this.children[i];
        }
    }

    return false;
};

MovieClip.prototype.getChildIndexById = function (id) {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            return i;
        }
    }

    return false;
};

MovieClip.prototype.removeChildById = function (id) {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            this.children.splice(i, 1);
            return;
        }
    }
};

MovieClip.prototype.swapChildIndex = function (id, placeAfter) {
    var i,
        child;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            child = this.children.splice(i, 1);
            break;
        }
    }

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == placeAfter) {
            this.children.splice(i + 1, 0, child[0]);
            break;
        }
    }
};

MovieClip.prototype.insertAtIndex = function (child, placeAfter) {
    var i;

    if (parseInt(placeAfter) === 0) {
        this.children.unshift(child);
    }

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == placeAfter) {
            this.children.splice(i + 1, 0, child);
            break;
        }
    }
};

MovieClip.prototype.containsMask = function () {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].isMask) {
            return true;
        }
    }

    return false;
};

MovieClip.prototype.matrix = function (newM) {
  //TODO:: set to newM if exists

  if (matrix) {
    return matrix;
  } else {
    return new Snap.Matrix();
  }
}

MovieClip.prototype.x = function (newX) {
  var _x = 0;

  if (matrix) {
    _x = martix.x();
  }

  return _x;
};

MovieClip.prototype.y = function (newY) {
  var _y = 0;

  if (matrix) {
    _y = martix.y();
  }

  return _y;
};

//mouse event handlers
MovieClip.prototype.mouseover = function (cb) {
  this.el.mouseover(cb);
};

MovieClip.prototype.mouseout = function (cb) {
  this.el.mouseout(cb);
};

MovieClip.prototype.mousedown = function (cb) {
  this.el.mousedown(cb);
};

MovieClip.prototype.mousemove = function (cb) {
  this.el.mousemove(cb);
};

MovieClip.prototype.click = function (cb) {
  this.el.click(cb);
};

//script methods
MovieClip.prototype.executeFrameScript = function (script) {
  eval("(function () {" + script + "}).call(this);");
};

MovieClip.prototype.removeFrameScript = function (id) {
  delete this._scripts[id];
};

MovieClip.prototype.addFrameScript = function (id, script) {
  this._scripts[id] = script;
};

MovieClip.prototype.getFrame = function (num) {
  var i;

  for (i = 0; i < this.m_timeline.Frame.length; i += 1) {
    if (this.m_timeline.Frame[i].num == num) {
      return this.m_timeline.Frame[i];
    }
  }
};

MovieClip.prototype._checkLoop = function () {
  if(this.m_currentFrameNo == this.m_frameCount)
  {
      if (!this.loops) {
          return;
      }
      this._loop();
  }
};

//playback methods
MovieClip.prototype._loop = function () {
    var frame,
        commands,
        children,
        i,
        found,
        cmData,
        type;

    console.log(this.id, 'LOOOP!!!');
    this.m_currentFrameNo = 0;

    frame = this.getFrame(this.m_currentFrameNo);

    if (!frame) {
      this.clearChildren();
      return;
    }

    //Get the commands for the first frame
    commands = frame.Command;

    // Iterate through all the elements in the display list
    // check if same instance exists in the first frame
    for (i = 0; i < this.children.length; i += 1) {

        found = false;
        child = this.children[i];

        for (c = 0; c < commands.length; ++c) {
            cmdData = commands[c];
            type = cmdData.cmdType;

            if (type == "Place") {
                if (parseInt(child.id) == parseInt(cmdData.objectId)) {
                    found = true;
                    break;
                }
            }
        }

        if (found === false) {
            command = new CMD.RemoveObjectCommand(child.id);
            this.commandList.push(command);
        }
    }
};

MovieClip.prototype.clearChildren = function () {
    var i,
        child,
        command;

    for (i = 0; i < this.children.length; i += 1) {
        child = this.children[i];
        command = new CMD.RemoveObjectCommand(child.id);
        this.commandList.push(command);
    }
};

MovieClip.prototype._animate = function () {
    var i;

    if (!this.playing) {
      return;
    }

    this.step_1_animTimeline();
    this.step_2_enterFrame();
    //this.step_3_addPending();
    this.step_4_frameConstructed();
    this.step_5_frameScripts();
    this.step_6_exitFrame();

    for(i = 0; i < this.children.length; i += 1)
    {
        if (this.children[i]._animate) {
            this.children[i]._animate();
        }
    }

    GP.purge();
};

MovieClip.prototype._runCommands = function (commands) {
  var c,
      cmdData,
      command,
      type,
      found;

  for(c = 0; c < commands.length; c += 1)
  {
      cmdData = commands[c];
      type = cmdData.cmdType;
      command = null;

      switch(type)
      {
          case "Place":

              found = this.getChildById(cmdData.objectId);

              if (!found) {
                  command = new CMD.PlaceObjectCommand(cmdData.charid, cmdData.objectId, cmdData.name, cmdData.placeAfter, cmdData.transformMatrix, cmdData.bounds);
                  this.commandList.push(command);
              } else {
                  command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
                  this.commandList.push(command);
                  command = new CMD.UpdateObjectCommand(cmdData.objectId, cmdData.placeAfter);
                  this.commandList.push(command);
              }

          break;
          case "Move":
              command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
              this.commandList.push(command);
          break;
          case "Remove":
              command = new CMD.RemoveObjectCommand(cmdData.objectId);
              this.commandList.push(command);
          break;
          case "UpdateZOrder":
              command = new CMD.UpdateObjectCommand(cmdData.objectId , cmdData.placeAfter);
              this.commandList.push(command);
          break;
          case "UpdateVisibility":
              command = new CMD.UpdateVisibilityCommand(cmdData.objectId , cmdData.visibility);
              this.commandList.push(command);
          break;
          case "UpdateColorTransform":
              command = new CMD.UpdateColorTransformCommand(cmdData.objectId , cmdData.colorMatrix);
              this.commandList.push(command);
          break;
          case "UpdateBlendMode":
            //  command = new CMD.UpdateBlendMode(cmdData.objectId , cmdData.blendMode);
            //  this.commandList.push(command);
          break;
          case "UpdateMask":
              command = new CMD.UpdateMaskCommand(cmdData.objectId , cmdData.maskTill);
              this.commandList.push(command);
          break;
          case "AddFrameScript":
            command = new CMD.AddFrameScriptCommand(cmdData.scriptId, cmdData.script);
            this.commandList.push(command);
          break;
          case "RemoveFrameScript":
            command = new CMD.RemoveFrameScriptCommand(cmdData.scriptId);
            this.commandList.push(command);
          break;
          case "SetFrameLabel":
            command = new CMD.SetFrameLabelCommand(cmdData.Name);
            this.commandList.push(command);
          break;
      }

  }

  if (this.containsMask) {
      command = new CMD.ApplyMaskCommand();
      this.commandList.push(command);
  }

  this.executeCommands(this.commandList, this.resourceManager);
}

//update timelien animations
MovieClip.prototype.step_1_animTimeline = function (seekMode, seekEnd) {

  if (typeof seekMode === "undefined") { seekMode = false; }
  if (typeof seekEnd === "undefined") { seekEnd = false; }

  var commands,
      frame;

  if (!this.playing) {
    return;
  }

  this.commandList = [];

  this._checkLoop();

  frame = this.getFrame(this.m_currentFrameNo);
  this.m_currentFrameNo++;

  this.log('step_1_animTimeline:', this.m_currentFrameNo);

  if (!frame) {
    return;
  }

  commands = frame.Command;
  this._runCommands(commands);

};

MovieClip.prototype.step_2_enterFrame = function () {
  //dispatch enter frame event
  //trigger on children
};

MovieClip.prototype.step_3_addPending = function () {

};

MovieClip.prototype.step_4_frameConstructed = function () {
  //dispatch frame constructed event
  //trigger on children
};

MovieClip.prototype.step_5_frameScripts = function () {
  //trigger framescripts
  //trigger on children
  this.log('step_5_frameScripts');

  for (var i in this._scripts) {
    this.executeFrameScript(this._scripts[i]);
  }
};

MovieClip.prototype.step_6_exitFrame = function () {
  //dispatch exit frame event
  //trigger on children
};

MovieClip.prototype.play = function () {
  this.playing = true;
}

MovieClip.prototype.stop = function () {
  this.playing = false;
};

MovieClip.prototype.gotoAndStop = function (fr) {
  this.log('gotoandstop')
  this._gotoAndPlayStop(fr, true);
};

MovieClip.prototype.gotoAndPlay = function (fr) {
  this.log('gotoandplay')
  this._gotoAndPlayStop(fr, false);
};

MovieClip.prototype._gotoAndPlayStop = function (frame, bStop) {

  //TODO::handle labels

  //if frame number is invalid, don't do anything

  this.log('GOTOANDPLAYSTOP::', frame, this.m_currentFrameNo, this.m_frameCount);

  if (frame < 1 || frame > this.m_frameCount) {
    return;
  }

  // If we are already at the destination frame, don't do anythin
  if (frame == this.m_currentFrameNo) {
    if (bStop === false) {
        this.play();
    } else {
        this.stop();
    }
    return;
  }

  // First ensure that the timeline is in a good state to start jumping around
  //this.step_3_addPending(true);
  this.play();

  // Loop around if necessary
  if (frame < this.m_currentFrameNo) {
    var bSeekEnd = (frame == 1);
    this._loopAround(true, bSeekEnd);
    //this.step_3_addPending(!bSeekEnd);
  }

  while (this.m_currentFrameNo < frame) {
    var bSeekEnd = (frame == this.m_currentFrameNo);
    this.step_1_animTimeline(true, bSeekEnd);

    for(var i = 0; i < this.children.length; i += 1)
    {
        if (this.children[i].step_1_animTimeline) {
            this.children[i].step_1_animTimeline(true, bSeekEnd);
        }
    }

    //this.step_3_addPending(!bSeekEnd);
  }

  if (bStop === false) {
      this.play();
  } else {
      this.stop();
  }

  this.step_4_frameConstructed();
  this.step_5_frameScripts();
  this.step_6_exitFrame();
}

MovieClip.prototype._loopAround = function (seekMode, seekEnd) {
  if (typeof seekMode === "undefined") { seekMode = false; }
  if (typeof seekEnd === "undefined") { seekEnd = false; }

  console.log(this.id, '////////LOOP AROUND!');

  this.commandList = [];
  this._checkLoop();
  this.m_currentFrameNo = 0;

  frame = this.getFrame(this.m_currentFrameNo);

  if (!frame) {
    return;
  }


  //Get the commands for the first frame
  commands = frame.Command;
  this._runCommands(commands);

}

MovieClip.prototype.executeCommands = function (commandList, resourceManager) {
    var i;

    for (i = 0; i < commandList.length ; i++)
    {
        if (commandList[i] !== undefined) {
             commandList[i].execute(this, resourceManager);
        }
    }
};

MovieClip.prototype.log = function () {
  if (this.id.indexOf('svg') > -1) { //only on main timeline
    var args = Array.prototype.slice.call(arguments);
    args.unshift(this.id.toUpperCase());
    //console.log.apply(console, args);
  }
}
