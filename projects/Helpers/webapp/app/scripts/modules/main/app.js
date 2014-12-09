
define([
    'jquery',
    'angular',
    'angular-route',
    'route',
    commonPath+'dialog/modalDialog',
    resUpdaterPath+'resUpdater'

],function($,angular,ngRoute,route,__){

    var app = angular.module("mainApp",["ngRoute","mainApp.common.modalDialog","modules.resUpdater"]);

    app.config(route);

    app.run(['$rootScope',function($rootScope){

        // $rootScope.dir = BlogInfo.url;
        // $rootScope.site = BlogInfo.site;
        // $rootScope.api = AppAPI.url;
        
        setTimeout(function(){
            //$rootScope.$emit("showModal");
        },1000);        

    }]);



    
    
    return app;
});