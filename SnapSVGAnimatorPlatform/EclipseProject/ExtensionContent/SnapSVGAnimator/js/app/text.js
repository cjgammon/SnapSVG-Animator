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

            var text,
                bbox,
                textBox,
                textX,
                textY,
                lineMode,
                fontSize,
                fontName,
                fontColor,
                textAlign,
                textAnchor,
                textIndent,
                textBaseline,
                textBounds;

            textBox = instance.el.g();

            if (bounds) {
                textBounds = bounds.split(',');
            } else {
                textBounds = [0, 0, 200, 100];
            }

            lineMode = data.behaviour.lineMode;
            textAlign = data.paras[0].alignment;
            textBaseline = lineMode == 'single' ? 'central' : 'auto';
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

            //rect
            if (data.behaviour.isBorderDrawn !== "false") {
                textRect = textBox.rect(textBounds[0], textBounds[1], textBounds[2], textBounds[3]);
                textRect.attr({
                    stroke: 'black',
                    fill: 'transparent'
                });
            }

            if (lineMode == 'single') {
                text = textBox.text(0, 0, data.txt);
                textY = parseFloat(textBounds[1]) + (parseFloat(textBounds[3]) / 2);
            } else {
                text = instance.multiLine(textBox, data, textBounds); 
                textY = parseFloat(textBounds[1]) - 10;
            }

            if (textAlign == 'left') {
                textX = parseFloat(textBounds[0]);
            } else {
                textX = parseFloat(textBounds[0]) + (parseFloat(textBounds[2]) / 2);
            }

            text.attr({
                'text-anchor': textAnchor,
                'dominant-baseline': textBaseline,
                'font-family': fontName,
                'font-size': fontSize,
                'letter-spacing': letterSpacing,
                'fill': fontColor
            });

            text.transform('translate(' + textX + ',' + textY + ')');
        };

        this.multiLine = function (textBox, data, textBounds) {
            
            var string = data.txt,
                spans = [],
                chars = '',
                substr,
                tempTxt,
                boundsWidth = parseFloat(textBounds[2]) - 10,
                sp,
                i = 0;

            while (i > -1) {
                chars += data.txt.charAt(i);
                tempTxt = textBox.text(0, 0, chars);
                bbox = tempTxt.getBBox();

                if (bbox.w > boundsWidth) {
                    newIndex = chars.lastIndexOf(' ');
                    substr = chars.slice(0, newIndex);
                    spans.push(substr);
                    i = i - (chars.length - substr.length) + 2;
                    chars = '';
                } else {
                    i += 1;
                }

                if (i >= data.txt.length) {
                    substr = chars.slice(0, newIndex);
                    spans.push(substr);
                    i = -1;
                }

                tempTxt.remove();
            }

            console.log(spans);

            text = textBox.text(0, 0, spans);
            sp = text.selectAll('tspan');
            sp.attr({
                'x': 0,
                'dy': data.paras[0].linespacing * 2.5
            });

            return text;
        };

        this.create();
    };

	return Text;
});
