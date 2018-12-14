function parse_and_color(text) {

    text.className = "prettyprint lang-c"
}

window.onload = function() {
    var examples = document.querySelectorAll("pre.example");

    for (var i=0; i<examples.length; ++i)
        parse_and_color(examples.item(i));

    var pretty = document.createElement("script");

    pretty.type = "text/javascript";
    pretty.src = "https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js";

    document.body.appendChild(pretty);
}
