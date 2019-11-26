#include <stdlib.h>
#include <stdio.h>

#include <aisl/aisl.h>

static void
minion_callback(const struct aisl_evt *evt, void *p_ctx);

static const struct aisl_cfg_srv m_srv = {
	.host = "0.0.0.0",
	.port = 8080
};

static const struct aisl_cfg m_cfg = {
	AISL_CFG_DEFAULTS,
	.srv = &m_srv,
	.srv_cnt = 1,
	.callback = minion_callback,
	.p_ctx = NULL
};

int
main(int argc, char **argv)
{
	AislInstance aisl;
	AislStatus status;

	if ((aisl = aisl_new(&m_cfg)) != NULL) {
		fprintf(stdout, "entering main loop\n");
		for (;;) {
			status = aisl_run_cycle(aisl);
			if (status != AISL_SUCCESS)
				aisl_sleep(aisl, 500);
		}
		aisl_free(aisl);
	} else {
		fprintf(stderr, "could not initialize AISL\n");
		return -1;
	}
	return 0;
}

static void
minion_callback(const struct aisl_evt *evt, void *p_ctx)
{
	AislStream s;
	const char html[] = 
		"<html>"
			"<head>"
				"<title>Hello World</title>"
				"<style>"
					"body {width: 100%; height: 100%}"
					"h1 {font-size: 4em}"
					".hello-world {"
						"position: absolute;"
						"top: 50%;"
						"left: 50%;"
						"width: 640px;"
						"height:200px;"
						"margin: -100px 0 0 -320px;"
						"text-align: center;"
					"}"
				"</style>"
			"</head>"
			"<body>"
				"<div class=\"hello-world\">"
					"<h1>Hello World</h1>"
					"<p>I am your new Minion</p>"
				"</div>"
			"</body>"
		"</html>";

	if (evt->code != AISL_EVENT_STREAM_REQUEST)
		return;

	s = evt->source;

	if (aisl_response(s, AISL_HTTP_OK, sizeof(html) - 1) == AISL_SUCCESS)
	{
		if (aisl_write(s, html, sizeof(html) - 1) != -1) {
			aisl_flush(s);
			return;
		}
	}

	aisl_reject(s);
}
