
define([
    'jquery',
    'angular',
    'angular-route',
    commonPath+'dialog/modalDialog'

],function($,angular,ngRoute,moduleModalDialog){
    var app = angular.module("mainApp",['ngRoute',"mainApp.common.modalDialog"]);

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