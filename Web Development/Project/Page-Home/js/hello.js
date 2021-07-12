function hello() {
  $.ajax({
    url:'https://freegeoip.app/json/',
    success: function(data){
          var helloString = 'Hello~  ' + data.ip + ' From:  ' + data.country_code;
          alert(helloString);
      },
    dataType: 'json'
  });
}
