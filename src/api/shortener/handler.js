/* eslint-disable require-jsdoc */

const ClientError = require('../../exceptions/ClientError');
/** shortener service
     */
class ShortenerHandler {
    /** initializer
     */
    constructor() {
        this.getIndexHandler = this.getIndexHandler.bind(this);
        this.postIndexHandler = this.postIndexHandler.bind(this);
        this.deleteIndexHandler = this.deleteIndexHandler.bind(this);
        this.redirectIndexHandler = this.redirectIndexHandler.bind(this);
    }
    /*
     * function handler
     * @param  {object} error
     */
    handleClientError(error) {
        if (error instanceof ClientError) {
            return res.status(e.statusCode).json({
                status: 'fail',
                message: e.message,
            });
        }
        // Server ERROR!
        return res.status(500).json({
            status: 'failed',
            message: 'internal server execption',
        });
    }
    /**
     * @param  {object} req
     * @param  {object} res
     */
    async getIndexHandler(req, res) {
        try {
            const {name = 'fulan', hobby = 'senyum'} = req.query;
            console.log(`Nama saya ${name} dan hobi saya ${hobby}`);
            return res.status(200).json({
                status: 'success',
                message: `nama saya ${name} dan hobi saya ${hobby}`,
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e);
        }
    }

    async redirectIndexHandler(req, res) {
        try {
            console.log(`Redirected to ${req.params.urlId}`);
            return res.status(200).json({
                status: 'success',
                message: `Redirected to ${req.params.urlId}`,
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e);
        }
    }

    async postIndexHandler(req, res) {
        try {
            const {name = 'fulan', hobby = 'senyum'} = req.query;
            console.log(`Nama saya ${name} dan hobi saya ${hobby}`);
            return res.status(200).json({
                status: 'success',
                message: `nama saya ${name} dan hobi saya ${hobby}`,
            });
        } catch (e) {
            console.log(e);
            this.handleClientError(e);
        }
    }

    async deleteIndexHandler(req, res) {
        try {
            const {name = 'fulan', hobby = 'senyum'} = req.query;
            console.log(`Nama saya ${name} dan hobi saya ${hobby}`);
            return res.status(200).json({
                status: 'success',
                message: `nama saya ${name} dan hobi saya ${hobby}`,
            });
        } catch (e) {
            this.handleClientError(e);
        }
    }
}

module.exports = ShortenerHandler;
