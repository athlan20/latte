define([
    'jquery',
    'text!'+resUpdaterPath+'main.tpl'
],function($,resUpdaterTpl){
    var controller = ['$rootScope','$scope','$http',function($rootScope,$scope,$http){
    	//alert(resUpdaterTpl);
    	//
    	
    	
    	$('#moduleView').append(resUpdaterTpl);
    }];
    return controller;
});