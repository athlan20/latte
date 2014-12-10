define([
    'jquery',
    'text!'+aboutPath+'main.tpl'
],function($,aboutTpl){
    var controller = ['$scope','$http',function($scope,$http){
    	//alert(resUpdaterTpl);
    	$('#moduleView').html(aboutTpl);
    }];
    return controller;
});