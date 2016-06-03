#include <libcouchbase/couchbase.h>
#include <cstdlib>

static void
flush_callback(lcb_t, int, const lcb_RESPCBFLUSH *resp)
{
    if (resp->rc != LCB_SUCCESS) {
        fprintf(stderr, "Couldn't flush bucket: %s\n",
            lcb_strerror(NULL, resp->rc));
    } else {
        printf("Flush successful\n");
    }
}

int main(int, char **)
{
    lcb_t instance;
    lcb_create(&instance, NULL);
    lcb_connect(instance);
    lcb_wait(instance);
    if (lcb_get_bootstrap_status(instance) != LCB_SUCCESS) {
        printf("Couldn't bootstrap!\n");
    }
    lcb_install_callback3(instance, LCB_CALLBACK_CBFLUSH, flush_callback);

    lcb_CMDCBFLUSH cmd = { 0 };

    /**
     * NOTE:
     * there is also an lcb_flush3() - that is a deprecated function and
     * should not be used.
     */
    lcb_cbflush3(instance, NULL, &cmd);
    lcb_wait(instance);
    lcb_destroy(instance);
    return 0;
}
