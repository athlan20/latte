<ul class="nav nav-tabs" role="tablist" ng-controller="mainController" main-directive>
	<li ng-repeat="router in routers" role="{{router.name}}"><a href="javascript:void(0)">{{router.title}}</a></li>
</ul>
<div ng-view>
</div>
