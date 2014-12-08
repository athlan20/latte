define([
    'jquery',
    commonPath+'config'

],function($,common,modalTpl){
    var controller = ['$rootScope','$scope','$http',function($rootScope,$scope,$http){
    	$rootScope.$on('showModal',function(){
    		
    		$('.modal').modal({});
    	})

	}]

    
    return controller;
});