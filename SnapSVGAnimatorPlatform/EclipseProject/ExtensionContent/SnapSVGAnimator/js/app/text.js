define(function (require) {
	
    var Text;

    Text = function (parentMC,resourceManager,charId,ObjectId,placeAfter,transform,bounds) {

        var instance = this,
            parentEl = parentMC.el;

        this.create = function () {
            var j,
                k,
                transformArray,
                transformMat;

            instance.el = parentMC.el.g();
            instance.id = ObjectId;
            instance.el.attr({'class': 'text', 'token': instance.id});

            instance.children = [];
            instance.isMask = false;
            instance.isMasked = false;
            instance.mask = null;
            instance.maskTill = null;

            //console.log(resourceManager.m_data.DOMDocument.Text);

            for (j = 0; j < resourceManager.m_data.DOMDocument.Text.length; j++)
            {
                if (resourceManager.m_data.DOMDocument.Text[j].charid == charId) 
                {
                    instance.addText(resourceManager.m_data.DOMDocument.Text[j]);
                }
            }

            transformArray = transform.split(",");
			transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
			instance.el.transform(transformMat);

            if (placeAfter && parseInt(placeAfter) !== 0) {
                afterMC = parentMC.getChildById(parseInt(placeAfter));
                afterMC.el.before(instance.el);
            } else {
                parentEl.add(instance.el);
            }
        };

        this.addText = function (data) {
            //console.log(data);

            var text,
                fontSize,
                fontName,
                fontColor,
                textAlign,
                textAnchor,
                textBounds;

            if (bounds) {
                textBounds = bounds.split(',');
            } else {
                textBounds = [0, 0, 200, 100];
            }

            console.log(textBounds);

            textAlign = data.paras[0].alignment;
            fontSize = data.paras[0].textRun[0].style.fontSize;
            fontName = data.paras[0].textRun[0].style.fontName;
            fontColor = data.paras[0].textRun[0].style.fontColor;
            letterSpacing = data.paras[0].textRun[0].style.letterSpacing;

            if (textAlign == 'left') {
                textAnchor = 'start';
            } else if (textAlign == 'center') {
                textAnchor = 'middle';
            } else if (textAlign == 'right') {
                textAnchor = 'end';
            }

            text = instance.el.text(0, 0, data.txt);
            text.attr({
                'text-anchor': textAnchor,
                'alignment-baseline': 'baseline',
                'font-family': fontName,
                'font-size': fontSize,
                'letter-spacing': letterSpacing,
                'fill': fontColor
            });

            bbox = text.getBBox();


            y = parseFloat(textBounds[1]) + (parseFloat(textBounds[3]) / 2);

            text.attr({
                //'y': (bbox.height / 2) - parseFloat(textBounds[1])
                'y': y
            });

            console.log((bbox.height / 2), parseFloat(textBounds[1]), (bbox.height / 2) - parseFloat(textBounds[1]));
        };

        this.create();
    };

	return Text;
});

/*
CreateText: function (root, s,resourceManager,charId,ObjectId,placeAfter,transform)
		{
			var textContainer = s.g();
			textContainer.attr({token: parseInt(ObjectId)});
			
			for(var b =0;b<resourceManager.m_data.DOMDocument.Text.length;b++)
			{
				if(resourceManager.m_data.DOMDocument.Text[b].charid == charId)
				{
					var displayString = resourceManager.m_data.DOMDocument.Text[b].txt;
					var txt = displayString.replace(/\\r/g,"\r");
					var fontFamily = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontName;
					var fontStyle = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontStyle;
					var fontSize = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontSize;
					var fontColor = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontColor;
					var textOutput = s.text(0, 0, txt);
					textOutput.attr({
						fill: fontColor, 
						fontFamily: fontFamily, 
						fontSize: fontSize, 
						fontStyle: fontStyle
					});
					
					textContainer.add(textOutput);
				}
			}
			
			var transformArray = transform.split(",");
			var TransformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
			textContainer.transform(TransformMat);
				
			if(s != undefined)
			{				
				if(placeAfter != 0)
				{	
					children = s.selectAll('[token="' + placeAfter + '"]');
					for (i = 0; i < children.length; i += 1) {
						if (children[i].parent().id == s.id) { //ensure child of current movie clip
							children[i].before(textContainer);
							break;
						}
					}	
				}
				else
				{
					s.add(textContainer);
				}	
				
			}
		}
		
	}
*/
