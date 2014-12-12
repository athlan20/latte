define([
    'jquery',
    'angular',
    resUpdaterPath+'controller',
    resUpdaterPath+'directive'

],function($,angular,resUpdaterController,resUpdaterDirective,tpl){

    var module = angular.module("modules.resUpdater",[]);
    var directive = module.directive("resupdater",resUpdaterDirective);
    var controller = module.controller("resUpdaterController",resUpdaterController);

    return module;
});