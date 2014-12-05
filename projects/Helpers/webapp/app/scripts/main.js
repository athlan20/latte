require.config({
    
    baseUrl: "./scripts",
    //alias libraries paths
    paths:{
        'angular':'bower_components/angular/angular',
        'angular-route':'bower_components/angular-route/angular-route',
        'jquery':'bower_components/jquery/dist/jquery.min',
        'text':'bower_components/requirejs-text/text',
        'jquery-lazyload':'bower_components/jquery.lazyload/jquery.lazyload.min',
        'bootstrap':'bower_components/bootstrap/dist/js/bootstrap.min'
    },
    shim:{
        "angular":{
            exports:"angular"
        },
        "angular-route":{
            deps:['angular'],
            exports:"ngRoute"
        },
        "bootstrap":{
            deps:['jquery'],
            exports:"bs"
        }
    }
});

/**
*   bootstrap angular use requirejs
*/
define([
    'angular',
    'jquery',
    'bootstrap',
    'modules/main/app',
    'modules/main/directive',
    'modules/main/controller',

],function(angular,$,bs,app){

    $().ready(function(){
        angular.bootstrap(document, ['mainApp']);
    });
    
});