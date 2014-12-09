define([
    'jquery',
    mainAppPath+'app',
    'angular',
    'appData'
],function($,app,angular,appData){
    var mainDirective = app.directive("mainDirective",['$rootScope','$timeout',function($rootScope,$timeout){

    	return{

    		scope: true,
    		link:function(scope,element,attrs){
    			$(element).find('a').off('click').on('click',function(event){
    				event.preventDefault()
  					$(this).tab('show')
    			});
                scope.routers = appData.routers;
    		}

    	}

    }]);

    
    return mainDirective;
});