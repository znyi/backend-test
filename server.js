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

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({extended: true}))

app.post('/sine/step1', (req, res)=>{
    console.log('step1')
    const outputString = CppAddon.sineStep1(JSON.stringify(req.body))
    const obj = JSON.parse(outputString)
    res.json(obj)
})
app.post('/sine/step2', (req, res)=>{
    console.log('step2') 
    const outputString = CppAddon.sineStep2(JSON.stringify(req.body))
    const obj = JSON.parse(outputString)
    res.json(obj)
})
app.post('/sine/step3', (req, res)=>{
    console.log('step3')
    const outputString = CppAddon.sineStep3(JSON.stringify(req.body))
    const obj = JSON.parse(outputString)
    res.json(obj)
})
app.post('/sine/step4', (req, res)=>{
    console.log('step4')
    const outputString = CppAddon.sineStep4(JSON.stringify(req.body))
    const obj = JSON.parse(outputString)
    res.json(obj)
})
app.get('/sine/stop', (req, res)=> { //to stop sine pdo
    console.log('sine stop')
    const outputString = CppAddon.sineStop()
    const obj = JSON.parse(outputString)
    res.json(obj)
})
app.post('/sine/output', (req, res)=> {
    console.log('sine output')
    const outputString = CppAddon.sineOutput(JSON.stringify(req.body))
    const obj = JSON.parse(outputString)
    res.json(obj)
})
app.listen(PORT, ()=>{
    console.log(`app listening at port ${PORT}.`)
})
