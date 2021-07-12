$('#google-form').submit(function (e) {
     //在這裡我們要先擋掉form默認事件
     e.preventDefault();

     // 確認必填欄位是否填寫
     if ($('#name').val() && $('#wish').val()) {
       $.ajax({
         url: "https://docs.google.com/forms/u/0/d/e/1FAIpQLSfioPeJTUZ9vSfptiIMq3OC19cp3SGBSVEZec_QE3tU71KSNQ/formResponse",
         crossDomain: true,
         data: {
           "entry.836460018": $('#name').val(),
           "entry.658175469": $('#wish').val()
         },
         type: "POST",
         dataType: "JSONP",
         complete: function () {
           //完成後把這些欄位清空
           $('#name').val('')
           $('#wish').val('')
         }
       });
     }
     else{
       alert("Please fill out name and wish");
     }
   });
