/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var App,
		Component = require('app/component');
		
    App = function () {
		var json,
			comp,
            script,
            callbackName = 'jsonp_callback_' + jsonfile.replace('.json', '');

        console.log("TESTING DEV");

        if (jsonfile) {
			//JSONP_Req(jsonfile);
			AJAX_JSON_Req(jsonfile);
        }

        window[callbackName] = function (data) {
            delete window[callbackName];
            document.body.removeChild(script);

            json = data;
            comp = new Component(jsonfile.replace('.json', ''), json, width, height);
            ready(comp);
        };


        function JSONP_Req(url) {
            script = document.createElement('script');
            script.src = url + (url.indexOf('?') >= 0 ? '&' : '?') + 'callback=' + callbackName;
            document.body.appendChild(script);
        }

		function AJAX_JSON_Req( url )
		{
		    var AJAX_req = new XMLHttpRequest();
		    AJAX_req.open( "GET", url, true );
		    AJAX_req.setRequestHeader("Content-type", "application/json");

		    AJAX_req.onreadystatechange = function()
		    {
		        if( AJAX_req.readyState == 4 && AJAX_req.status == 200 )
		        {
					json = JSON.parse(AJAX_req.responseText);
			    	comp = new Component(jsonfile.replace('.json', ''), json, width, height);
					ready(comp);
		        }
		    }
		    AJAX_req.send();
		}

    };

	return new App();
});
