/**
 * Add gobals here
 */
var seconds 	= null;
// var otaTimerVar =  null;
var wifiConnectInterval = null;

/**
 * Initialize functions here.
 */
$(document).ready(function(){
	// getUpdateStatus();
	// startLocalTimeInterval();
    getConnectInfo();
	$("#connect_wifi").on("click", function(){
		checkCredentials();
	}); 
	$("#disconnect_wifi").on("click", function(){
		disconnectWifi();
	}); 
});   


/**************************
**		CONNECTING  	 **
**************************/

/**
 * Clears the connection status interval.
 */
function stopWifiConnectStatusInterval()
{
	if (wifiConnectInterval != null)
	{
		clearInterval(wifiConnectInterval);
		wifiConnectInterval = null;
	}
}

/**
 * Gets the WiFi connection status.
 */
function getWifiConnectStatus()
{
	var xhr = new XMLHttpRequest();
	var requestURL = "/wifiConnectStatus";
	xhr.open('POST', requestURL, false);
	xhr.send('wifi_connect_status');
	
    // 4 = sent
	if (xhr.readyState == 4 && xhr.status == 200)
	{
		var response = JSON.parse(xhr.responseText);
		
		document.getElementById("wifi_connect_status").innerHTML = "Connecting...";
		
		if (response.wifi_connect_status_json == 2)
		{
			document.getElementById("wifi_connect_status").innerHTML = "<h4 class='rd'>Failed to Connect. Please check your AP credentials and compatibility</h4>";
			stopWifiConnectStatusInterval();
		}
		else if (response.wifi_connect_status_json == 3)
		{
			document.getElementById("wifi_connect_status").innerHTML = "<h4 class='gr'>Connection Success!</h4>";
			stopWifiConnectStatusInterval();
            getConnectInfo();
		}
	}
}

/**
 * Starts the interval for checking the connection status.
 */
function startWifiConnectStatusInterval()
{
	wifiConnectInterval = setInterval(getWifiConnectStatus, 2800);
}

/**
 * Connect WiFi function called using the SSID and password entered into the text fields.
 */
function connectWifi()
{
	// Get the SSID and password
	selectedSSID = $("#connect_ssid").val();
	pwd = $("#connect_pass").val();
	
	$.ajax({
		url: '/wifiConnect.json',
		dataType: 'json',
		method: 'POST',
		cache: false,
		data: JSON.stringify({ c_ssid: selectedSSID, c_pwd: pwd}),
	});
	
	startWifiConnectStatusInterval();
}

/**
 * Checks credentials on connect_wifi button click.
 */
function checkCredentials()
{
	errorList = "";
	credsOk = true;
	
	selectedSSID = $("#connect_ssid").val();
	pwd = $("#connect_pass").val();
	
	if (selectedSSID == "")
	{
		errorList += "<h4 class='rd'>SSID cannot be empty!</h4>";
		credsOk = false;
	}
	if (pwd == "")
	{
		errorList += "<h4 class='rd'>Password cannot be empty!</h4>";
		credsOk = false;
	}
	
	if (credsOk == false)
	{
		$("#wifi_connect_credentials_errors").html(errorList);
	}
	else
	{
		$("#wifi_connect_credentials_errors").html("");
		connectWifi();    
	}
}

/**
 * Shows the WiFi password if the box is checked.
 */
function showPassword()
{
	var x = document.getElementById("connect_pass");
	if (x.type === "password")
	{
		x.type = "text";
	}
	else
	{
		x.type = "password";
	}
}

/**
 * Gets the connection information for displaying on the web page
 */
function getConnectInfo()
{
    console.log("should have displayed something here dude");
    $.getJSON('/wifiConnectInfo.json', function(data)
    {
        $("#connected_ap_label").html("Connected to: "); 
        $("#connected_ap").text(data["ap"]);
        
        $("#ip_address_label").html("IP Adress: "); 
        $("#wifi_connect_ip").text(data["ip"]);
        
        $("#netmask_label").html("Netmask: "); 
        $("#wifi_connect_netmask").text(data["netmask"]);
        
        $("#gateway_label").html("Gateway: "); 
        $("#wifi_connect_gateway").text(data["gateway"]);

        document.getElementById('disconnect_wifi').style.display = 'block';         
    });
}

/**
 * Disconnect WiFi once the disconnect button is pressed, and reloads the web page.
 */
function disconnectWifi()
{
	$.ajax({
		url: '/wifiDisconnect.json',
		dataType: 'json',
		method: 'DELETE',
		cache: false,
		data: { 'timestamp': Date.now() }
	});
	// Update the web page
	setTimeout("location.reload(true);", 2000);
}

/**
 * Sets the interval for displaying local time.
 */
function startLocalTimeInterval()
{
	setInterval(getLocalTime, 10000);
}

/**
 * Gets the local time.
 * @note connect the ESP32 to the internet and the time will be updated.
 */
function getLocalTime()
{
	$.getJSON('localTime.json', function(data)
	{
		$("#local_time").text(data["time"]);
	});
}