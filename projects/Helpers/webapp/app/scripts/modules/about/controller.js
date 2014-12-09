define([
    'jquery',
    'text!'+resUpdaterPath+'main.tpl'
],function($,resUpdaterTpl){
    var controller = ['$scope','$http',function($scope,$http){
    	//alert(resUpdaterTpl);
    	$('#moduleView').html(resUpdaterTpl);
    }];
    return controller;
});