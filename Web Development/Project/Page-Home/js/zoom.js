$(function() {
  $(".zoom").click(function() {
    if ($(this).hasClass("before")) {
      $(this).removeClass("before").addClass("after");
    } else {
      $(this).removeClass("after").addClass("before");
    }
  })
})
