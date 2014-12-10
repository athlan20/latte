
define([
    'jquery',
    'angular',
    'angular-route',
    'route',
    commonPath+'dialog/modalDialog',
    resUpdaterPath+'resUpdater',
    aboutPath+'about'

],function($,angular,ngRoute,route,__,__){

    var app = angular.module("mainApp",["ngRoute","mainApp.common.modalDialog","modules.resUpdater","modules.about"]);

    app.config(route);
    // 
    //app.config(["$rootScope",function(){}]);

    app.run(['$rootScope',function($rootScope){

        // $rootScope.dir = BlogInfo.url;
        // $rootScope.site = BlogInfo.site;
        // $rootScope.api = AppAPI.url;
        
        setTimeout(function(){
            //$rootScope.$emit("showModal");
        },1000);

        //路由监听
        $rootScope.$on('$routeChangeSuccess', function(param,routeObj){
            var router = routeObj.$$route;
            if(router){
                var routerName = router.controller.replace("Controller",'');
                $rootScope.$emit("evtSelectTab",routerName);
            }

            // window.cefQuery({
            //     request: 'CallNative:' + '132',//document.getElementById("message").value,
            //     onSuccess: function(response) {
            //         alert(response)
            //       //document.getElementById('result').value = 'Response: '+response;
            //     },
            //     onFailure: function(error_code, error_message) {}
            // });
        });

    }]);



    
    
    return app;
});