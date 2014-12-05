
define([
    'jquery',
    'angular',
    'angular-route'

],function($,angular,ngRoute){
    var app = angular.module("mainApp",['ngRoute']);

    app.run(['$rootScope',function($rootScope){

        // $rootScope.dir = BlogInfo.url;
        // $rootScope.site = BlogInfo.site;
        // $rootScope.api = AppAPI.url;
        

    }]);
    
    return app;
});