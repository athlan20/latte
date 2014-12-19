define([
    'jquery',
    mainAppPath+'app',
    'angular',
    'appData'
],function($,app,angular,appData){
    var mainDirective = app.directive("mainDirective",['$rootScope','$timeout','$location','$compile',
        function($rootScope,$timeout,$location,$compile){

    	return{

    		scope: true,
    		link:function(scope,element,attrs){
                scope.routers = appData.routers;

                setTimeout(function(){
                    $(element).find('a').off('click').on('click',function(event){
                        event.preventDefault();
                        var role = $(this).parent().attr('role');
                        window.location = "#/"+role;
                        //$(this).tab('show')
                    });

                },0);


                //事件注册
                $rootScope.$on("evtSelectTab",function(param,selectName){
                    $(element).find('li[role="'+selectName+'"]').find('a').tab('show');
                })

    		}

    	}

    }]);

    
    return mainDirective;
});