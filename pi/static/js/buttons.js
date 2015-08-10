window.LEDcontrol = {
    sendCommand: function (event, command) {
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.open("POST", "/", true);
        xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xmlhttp.send("command=" + command);
    }
};

document.addEventListener('DOMContentLoaded', function() {
    document.getElementById("black-btn").addEventListener("click", function () {
        LEDcontrol.sendCommand(event, "black");
    });
    document.getElementById("chase-btn").addEventListener("click", function () {
        LEDcontrol.sendCommand(event, "chase");
    });
});
