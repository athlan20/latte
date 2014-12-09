define([
    'jquery',
    'angular',
    aboutPath+'controller'

],function($,angular,aboutController){
    var module = angular.module("modules.aboutPath",[]);
    // var directive = module.directive("modalDirective",modalDirective);
    var controller = module.controller("aboutController",aboutController);
    
    return module;
});