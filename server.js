import express from 'express';
import bodyParser from 'body-parser';
import cors from 'cors'
import { createRequire } from 'node:module'

//set up cpp addon
const require = createRequire(import.meta.url)
const CppAddon = require('./build/Release/CppAddon.node')

//set up web back end
const app = express()
const PORT = 5000

const corsOptions = {
    origin: 'http://localhost:3000', //'https://210.181.65.166:3100', //
    optionsSuccessStatus: 200 // some legacy browsers (IE11, various SmartTVs) choke on 204
}
app.use(cors(corsOptions))

app.use(bodyParser.urlencoded({extended: true}))
app.use(bodyParser.json())

app.get('/sdo/command', (req, res)=> {
    const sdo_a = req.query.sdo_a //not parseInt bcs i need to pass string to cpp function
    const outputString = CppAddon.getSdoCommand(sdo_a)
    const sdo_input_buffer = JSON.parse(outputString)
    res.json(sdo_input_buffer)
})
app.get('/sdo/output', (req, res)=> {
    const sdo_output_buffer = req.query.sdo_output_buffer //not JSON.parse bcs i need to pass string to cpp function
    const outputString = CppAddon.getSdoOutput(sdo_output_buffer)
    const sdo_a_increased = JSON.parse(outputString)
    res.json(sdo_a_increased)
})

app.get('/pdo/command/exec', (req, res)=> {
    const outputString = CppAddon.getPdoExecCommand()
    const pdo_input_buffer = JSON.parse(outputString)
    res.json(pdo_input_buffer)
})
app.get('/pdo/command/stop', (req, res)=> {
    const outputString = CppAddon.getPdoStopCommand()
    const pdo_input_buffer = JSON.parse(outputString)
    res.json(pdo_input_buffer)
})
app.post('/pdo/output', (req, res)=> {
    const pdo_value_chunk = req.body //an array containing pdo raw values
    const pdo_output_chunk = pdo_value_chunk.map((elem)=>{
        const inputString = JSON.stringify(elem)
        const outputString = CppAddon.getPdoOutput(inputString)
        return JSON.parse(outputString)
    })
    res.json(pdo_output_chunk)
})

app.listen(PORT, ()=>{
    console.log(`app listening at port ${PORT}.`)
})
