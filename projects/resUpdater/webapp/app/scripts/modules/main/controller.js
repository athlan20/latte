define([
    'jquery',
    mainAppPath+'app',
    'text!'+mainAppPath+'mainTab.tpl'

],function($,app,mainTabTpl){
    var mainController = app.controller("mainController",['$scope',function($scope){
    }]);

    $('#main').append(mainTabTpl);
    return mainController;
});