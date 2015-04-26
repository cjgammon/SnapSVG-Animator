/*global require*/

require.config({
    shim: {
    },

    paths: {
	    tweenmax: 'vendor/greensock/TweenMax',
		snapplugin: 'vendor/greensock/plugins/SnapPlugin',
        jquery: 'vendor/jquery/dist/jquery',
        snap: 'vendor/snap.svg/dist/snap.svg'
    }
});

require(['jquery', 'app']);
