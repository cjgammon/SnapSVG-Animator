/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var App,
		Component = require('app/component');
		
    App = function () {
		var json,
			comp;
	
		if (jsonfile) {
			AJAX_JSON_Req(jsonfile);
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
