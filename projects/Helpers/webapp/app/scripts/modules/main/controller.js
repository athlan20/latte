define([
    'jquery',
    mainAppPath+'app',
    'text!'+mainAppPath+'mainTab.tpl'

],function($,app,mainTabTpl){
    var mainController = app.controller("mainController",['$scope','$http',function($scope,$http){

    }]);
    $('#main').append(mainTabTpl);
    return mainController;
});