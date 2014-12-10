define([
    'jquery',
    'angular',
    resUpdaterPath+'controller',
    resUpdaterPath+'directive'

],function($,angular,resUpdaterController,resUpdaterDirective){
    var module = angular.module("modules.resUpdater",[]);
    var directive = module.directive("resUpdaterDirective",resUpdaterDirective);
    var controller = module.controller("resUpdaterController",resUpdaterController);
    
    return module;
});