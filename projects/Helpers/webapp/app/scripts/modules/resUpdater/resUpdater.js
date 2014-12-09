define([
    'jquery',
    'angular',
    resUpdaterPath+'controller'

],function($,angular,modalController){
    var module = angular.module("modules.resUpdater",[]);
    // var directive = module.directive("modalDirective",modalDirective);
    var controller = module.controller("resUpdaterController",modalController);
    
    return module;
});