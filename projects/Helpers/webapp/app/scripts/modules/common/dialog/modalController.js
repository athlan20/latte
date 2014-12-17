define([
    'jquery',
    commonPath+'config'

],function($,common,modalTpl){
    var controller = ['$rootScope','$scope','$http',function($rootScope,$scope,$http){
    	$rootScope.$on('showModal',function(){
    		
    		$('.modal').modal({});
    	})

    	//native用
        $rootScope.$on("showAlert",function(target,param){
        	$scope.content = param.msg;
        	$scope.$digest();
            $('.modal').modal({});
        })
	}]

    
    return controller;
});