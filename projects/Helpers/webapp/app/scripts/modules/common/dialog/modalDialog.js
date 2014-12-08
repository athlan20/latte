define([
    'jquery',
    'angular',
    'text!'+commonPath+'dialog/modal.tpl',
    commonPath+'dialog/modalDirective',
    commonPath+'dialog/modalController'

],function($,angular,modalTpl,modalDirective,modalController){
    var module = angular.module("mainApp.common.modalDialog",[]);
    var directive = module.directive("modalDirective",modalDirective);
    var controller = module.controller("modalController",modalController);
    
    $('#main').append(modalTpl);
    return module;
});