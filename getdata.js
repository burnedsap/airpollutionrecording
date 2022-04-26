
function getData() {
  let url =
    "https://sheets.googleapis.com/v4/spreadsheets/1-FL7Jpvgkb4nATc9-zvB2ACzLROcbjIAaAlj2q_mFPg/values/Sheet1?key=AIzaSyCh9Qi0ASAJtbuIBez37uCC6QBwF_JwbuM";

  request = new XMLHttpRequest();
  request.open("GET", url);
  request.onload = function () {
    if (request.status >= 200 && request.status < 400) {
      // Success!
      let data = JSON.parse(request.responseText);
      loadIn(data);
      // console.log(data);
      return data;
    } else {
      console.log("error");
      // We reached our target server, but it returned an error
    }
  };
  request.onerror = function () {
    // There was a connection error of some sort
  };
  request.send();
}

let latestValue;

function loadIn(data) {
  // console.log(data.values);
  console.log(data.values.length);
  // console.log(data.values[data.values.length-1]);
  latestValue = data.values[data.values.length-1];
}
