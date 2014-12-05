define([
    'jquery',
    mainAppPath+'app',
    'angular'
],function($,app){
    var mainDirective = app.directive("mainDirective",['$rootScope','$timeout',function($rootScope,$timeout){

    	return{

    		scope: true,
    		link:function(scope,element,attrs){
    			$(element).find('a').off('click').on('click',function(event){
    				event.preventDefault()
  					$(this).tab('show')
    			});

    		}

    	}

    }]);

    
    return mainDirective;
});