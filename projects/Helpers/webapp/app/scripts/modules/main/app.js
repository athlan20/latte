
define([
    'jquery',
    'angular',
    'angular-route',
    'route',
    commonPath+'dialog/modalDialog',
    resUpdaterPath+'resUpdater',
    aboutPath+'about'

],function($,angular,ngRoute,route,__,__,__,tpl){
    var app = angular.module("mainApp",["ngRoute","mainApp.common.modalDialog","modules.about","modules.resUpdater"]);
    app.config(route);

    app.run(['$rootScope','$compile',function($rootScope,$compile){
        
        window.rootScope = $rootScope;

        $rootScope.$on("nativeCall",function(){
            console.log(arguments.length);
        })

        //路由监听
        $rootScope.$on('$routeChangeSuccess', function(param,routeObj){
            var router = routeObj.$$route;
            if(router){
                var routerName = router.controller.replace("Controller","");
                $rootScope.$emit("evtSelectTab",routerName);
            }
        });

    }]);
    
    
    return app;
});