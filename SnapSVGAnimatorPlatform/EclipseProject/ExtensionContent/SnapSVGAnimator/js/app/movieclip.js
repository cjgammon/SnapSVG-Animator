
var MovieClip = function (parentMC, commandTimeline, resourceManager, objectID, name, placeAfter, transform) {
    var i,
        transformMat,
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

    if(this.transform !== undefined)
    {
        transformData = this.transform;
        transformArray = transformData.split(",");
        transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
        this.el.transform(transformMat);
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

//playback methods
MovieClip.prototype.loop = function (commandList) {
    var frame,
        commands,
        children,
        i,
        found,
        cmData,
        type;

    this.m_currentFrameNo = 0;

    //loop through keyframes to find first frame
    for (i = 0; i < this.m_timeline.Frame.length; i += 1) {

        if (parseInt(this.m_timeline.Frame[i].num) == this.m_currentFrameNo) {
            frame = this.m_timeline.Frame[i];
            break;
        } else if (i >= this.m_timeline.Frame.length - 1) {
            //the first frame is empty
            this.clearChildren(commandList);
            return;
        }
    }

    if (!frame) {
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
            commandList.push(command);
        }
    }
};

MovieClip.prototype.clearChildren = function (commandList) {
    var i,
        child,
        command;

    for (i = 0; i < this.children.length; i += 1) {
        child = this.children[i];
        command = new CMD.RemoveObjectCommand(child.id);
        commandList.push(command);
    }
};

MovieClip.prototype.runFrame = function () {
    var frame,
        commandList = [],
        i,
        commands,
        found,
        command,
        c,
        cmdData,
        type;

    //play movieclips
    for(i = 0; i < this.children.length; i += 1)
    {
        if (this.children[i].runFrame) {
            this.children[i].runFrame();
        }
    }

    if (!this.playing) {
      return;
    }

    //check to handle looping of movieclip
    if(this.m_currentFrameNo == this.m_frameCount + 1)
    {
        if (!this.loops) {
            return;
        }
        this.loop(commandList);
    }

    for (i = 0; i < this.m_timeline.Frame.length; i += 1) {
        if (parseInt(this.m_timeline.Frame[i].num) == this.m_currentFrameNo) {
            frame = this.m_timeline.Frame[i];
            break;
        } else if (i >= this.m_timeline.Frame.length - 1) {
            if (this.m_currentFrameNo === 0) { //first frame is empty (execute any remove commands)
                this.executeCommands(commandList, this.resourceManager);
            }
            this.stepFrame();
            this.m_currentFrameNo += 1;
            return;
        }
    }

    if (!frame) {
        return;
    }

    commands = frame.Command;
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
                    commandList.push(command);
                } else {
                    command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
                    commandList.push(command);
                    command = new CMD.UpdateObjectCommand(cmdData.objectId, cmdData.placeAfter);
                    commandList.push(command);
                }

            break;
            case "Move":
                command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
                commandList.push(command);
            break;
            case "Remove":
                command = new CMD.RemoveObjectCommand(cmdData.objectId);
                commandList.push(command);
            break;
            case "UpdateZOrder":
                command = new CMD.UpdateObjectCommand(cmdData.objectId , cmdData.placeAfter);
                commandList.push(command);
            break;
            case "UpdateVisibility":
                command = new CMD.UpdateVisibilityCommand(cmdData.objectId , cmdData.visibility);
                commandList.push(command);
            break;
            case "UpdateColorTransform":
                command = new CMD.UpdateColorTransformCommand(cmdData.objectId , cmdData.colorMatrix);
                commandList.push(command);
            break;
            case "UpdateMask":
                command = new CMD.UpdateMaskCommand(cmdData.objectId , cmdData.maskTill);
                commandList.push(command);
            break;
            case "AddFrameScript":
              command = new CMD.AddFrameScriptCommand(cmdData.scriptId, cmdData.script);
              commandList.push(command);
            break;
            case "RemoveFrameScript":
              command = new CMD.RemoveFrameScriptCommand(cmdData.scriptId);
              commandList.push(command);
            break;
            case "SetFrameLabel":
              command = new CMD.SetFrameLabelCommand(cmdData.Name);
              commandList.push(command);
            break;
        }

    }

    if (this.containsMask) {
        command = new CMD.ApplyMaskCommand();
        commandList.push(command);
    }

    this.executeCommands(commandList, this.resourceManager);

    this.stepFrame();

    this.m_currentFrameNo++;

    GP.purge();
};

MovieClip.prototype.stepFrame = function () {
  this.step_1_animTimeline();
  this.step_2_enterFrame();
  //this.step_3_addPending();
  this.step_4_frameConstructed();
  this.step_5_frameScripts();
  this.step_6_exitFrame();
};

MovieClip.prototype.step_1_animTimeline = function () {

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

MovieClip.prototype.gotoAndStop = function (num) {
  console.log('gotoandstop', num);
  //not working plays instead
  this.m_currentFrameNo = num;
  this.playing = false;
};

MovieClip.prototype.gotoAndPlay = function (num) {
  this.m_currentFrameNo = num;
  this.playing = true;
};

MovieClip.prototype.executeCommands = function (commandList, resourceManager) {
    var i;

    for (i = 0; i < commandList.length ; i++)
    {
        if (commandList[i] !== undefined) {
             commandList[i].execute(this, resourceManager);
        }
    }
};
