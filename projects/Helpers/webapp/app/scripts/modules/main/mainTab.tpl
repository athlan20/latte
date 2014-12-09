<ul class="nav nav-tabs" role="tablist" ng-controller="mainController" main-directive>
	<li ng-repeat="router in routers" role="presentation"><a href="#{{router.router}}">{{router.title}}</a></li>
<!--   <li role="presentation" class="active"><a href="#">Home</a></li>
  <li role="presentation"><a href="#">Profile</a></li>
  <li role="presentation"><a href="#">Messages</a></li> -->
</ul>
<ng-view id="moduleView">
	
</ng-view>